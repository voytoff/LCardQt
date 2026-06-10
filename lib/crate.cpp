#include "crate.h"
#include "lcard.h"
#include "lctypes.h"
#include "ltr11.h"
#include <qdebug.h>

#include <QHostAddress>
#include <QString>

Crate::Crate(const QString &addr, QObject *parent)
  : QObject{parent}
  , address(addr)
  , ltr(new TLTR{0})
  , modules(new QHash<int, ILCModule*>()) {
  ltr->saddr = LTRD_ADDR_DEFAULT;
  ltr->sport = LTRD_PORT_DEFAULT;
  if (addr.length() > 0)
    ltr->saddr = QHostAddress(address).toIPv4Address();
  result = LTR_Init(ltr);
}

/*static*/int Crate::addresses(QList<LCCrateInfo> &array) {
  TLTR ltr = {0};
  memset(&ltr, 0, sizeof(TLTR));
  DWORD entries_found = 0;
  DWORD entries_returned = 0;
  TLTR_CRATE_IP_ENTRY *zero = nullptr;
  INT result = LTR_Init(&ltr);
  if (result == LTR_OK) {
    result = LTR_Open(&ltr);
    if (result == LTR_OK) {
      // byte arr[,]; LTR_GetCrates(ltr, array);
      result = LTR_GetListOfIPCrates(&ltr, 0, 0, 0, &entries_found, &entries_returned, zero);
      if (result == LTR_OK && entries_found > 0) {
        TLTR_CRATE_IP_ENTRY tmp[entries_found];
        std::memset(tmp, 0, entries_found * sizeof(TLTR_CRATE_IP_ENTRY));
        result = LTR_GetListOfIPCrates(&ltr, 1, 0, 0, &entries_found, &entries_returned, tmp);
        if (result == LTR_OK && entries_found > 0) {
          for (int i = 0; i < entries_found; i++) {
            LCCrateInfo info = {tmp[i].ip_addr, tmp[i].flags, QString(tmp[i].serial_number), tmp[i].status};
            array.append(info);
          }
        } else {
          qDebug() << LCard::getErrorString(result);
        }
      } else {
        qDebug() << LCard::getErrorString(result);
      }
    }
  }
  return result;
}

bool Crate::open() {
  result = LTR_Open(ltr);
  if (result == LTR_OK) {
    uint v = version();
    if (v > 0) ver = QString("%1.%2.%3.%4").arg((v >> 24) & 0xFF).arg((v >> 16) & 0xFF).arg((v >> 8) & 0xFF).arg(v & 0xFF);
    if (init()) {
      auto modules = hardware();
      foreach (auto key, modules.keys()) {
        auto m = modules.value(key);
        m->open(key);
      }
    }
  }
  return result == LTR_OK;
}

bool Crate::init() {
  modules->clear();
  result = LTR_Open(ltr);
  if (result == LTR_OK) {
    WORD mid[MODULE_MAX];
    result = LTR_GetCrateModules(ltr, mid);
    if (result == LTR_OK) {
      for (int slot = 1; slot <= MODULE_MAX; slot++) {
        auto type = mid[slot]&0xFF;
        if (type > 0) {
          auto m = module(slot, mid[slot]&0xFF);
          if (m) {
            if (m->open(slot)) modules->insert(slot, m);
            else {
              result = m->error();
              qDebug() << slot << lastError();
            }
          }
        }
      }
    }
  }
  qDebug() << lastError();
  return result == LTR_OK;
}

ILCModule* Crate::module(const int &slot, const int &type) {
  switch (type) {
  case 11:
    return new class ltr11(this);
  case 114:
  default:
    break;
  }
  return nullptr;
}

bool Crate::close() {
  result = LTR_Close(ltr);
  return result == LTR_OK;
}

bool Crate::start(LCParameters *params) {
  this->params = params;
  bool res = true;
  foreach (auto key, params->keys()) {
    auto m = modules->value(key);
    if (m) {
      connect(m->base(), &LTRBase::dataReady, this, [=](ILCModule *module, const int &count, double *data) {
        emit dataReady(module, count, data);
      });
      res = m->start(params->value(key));
      if (!res) {
        result = m->error();
        qDebug() << lastError();
        break;
      }
    } else qDebug() << "Отсутствует слот с номером:" << key;
  }
  return res;
}

bool Crate::stop() {
  bool res = true;
  foreach (auto key, params->keys()) {
    auto m = modules->value(key);
    if (m) {
      res = m->stop();
      if (!res) {
        result = m->error();
        qDebug() << lastError();
        break;
      }
    } else qDebug() << "Отсутствует слот с номером:" << key;
  }
  return res;
}

bool Crate::opened() {
  result = LTR_IsOpened(ltr);
  return result == LTR_OK;
}

uint Crate::version() {
  DWORD version;
  result = LTR_GetServerVersion(ltr, &version);
  return result == LTR_OK ? version : 0;
}
