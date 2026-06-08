#include "crate.h"
#include "lcard.h"
#include "lctypes.h"
#include "ltr11.h"
#include <qdebug.h>

#include <QHostAddress>
#include <QString>

Crate::Crate(QObject *parent)
  : QObject{parent}
  , ltr(new TLTR{0})
  , hardware(new QHash<int, LCModuleInfo*>()) {
  LTR_Init(ltr);
}

/*static*/int Crate::addresses(QList<LCCrateInfo> &array) {
  TLTR ltr = {0};
  memset(&ltr, 0, sizeof(TLTR));
  DWORD entries_found = 0;
  DWORD entries_returned = 0;
  TLTR_CRATE_IP_ENTRY *zero = nullptr;
  INT res = LTR_Init(&ltr);
  if (res == LTR_OK) {
    res = LTR_Open(&ltr);
    if (res == LTR_OK) {
      // byte arr[,]; LTR_GetCrates(ltr, array);
      res = LTR_GetListOfIPCrates(&ltr, 0, 0, 0, &entries_found, &entries_returned, zero);
      if (res == LTR_OK && entries_found > 0) {        
        TLTR_CRATE_IP_ENTRY tmp[entries_found];
        std::memset(tmp, 0, entries_found * sizeof(TLTR_CRATE_IP_ENTRY));
        res = LTR_GetListOfIPCrates(&ltr, 1, 0, 0, &entries_found, &entries_returned, tmp);
        if (res == LTR_OK && entries_found > 0) {
          for (int i = 0; i < entries_found; i++) {
            LCCrateInfo info = {tmp[i].ip_addr, tmp[i].flags, QString(tmp[i].serial_number), tmp[i].status};
            array.append(info);
          }
        } else {
          qDebug() << LCard::getErrorString(res);
        }
      } else {
        qDebug() << LCard::getErrorString(res);
      }
    }
  }
  return res;
}

bool Crate::open(const QString &addr) {
  address = addr;
  ltr->saddr = QHostAddress(address).toIPv4Address();
  return LTR_Open(ltr) == LTR_OK;
}

bool Crate::close() {
  return LTR_Close(ltr) == LTR_OK;
}

bool Crate::start()
{

}

bool Crate::stop()
{

}

bool Crate::isOpened() {
  return LTR_IsOpened(ltr) == LTR_OK;
}

uint Crate::version() {
  DWORD version;
  INT res = LTR_GetServerVersion(ltr, &version);
  return res == LTR_OK ? version : 0;
}

LCModuleInfo* Crate::info(const int &slot, const int &type) {
  if (hardware->contains(type)) return hardware->value(type);
  switch (type) {
  case 11: {
    ILCModule *ltr11 = new class ltr11();
    ltr11->open(address, slot);
    auto info = ltr11->info();
    hardware->insert(type, info);
    return info;
  }
    break;
  case 114:
    break;
  default:
    break;
  }
  return nullptr;
}

QList<LCModuleInfo*> Crate::modules() {
  INT res;
  res = LTR_Init(ltr);
  if (res == LTR_OK) {
    res = LTR_Open(ltr);
    if (res == LTR_OK) {
      WORD mid[MODULE_MAX];
      res = LTR_GetCrateModules(ltr, mid);
      if (res == LTR_OK) {
        QList<LCModuleInfo*> result;
        for(int i = 0; i < MODULE_MAX; i++) {
          auto type = mid[i]&0xFF;
          if (type > 0) {
            auto module = info(i, mid[i]&0xFF);
            result.append(module);
          }
        }
        return result;
      }
    }
  }
  qDebug() << QString(decoder.decode(LTR_GetErrorString(res)));
  return {};
}

void a() {

}
