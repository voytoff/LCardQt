#include "crate.h"
#include "lcard.h"
#include "ltr11.h"
#include <qdebug.h>

#include <QHostAddress>
#include <QString>
#include <QChar>

Crate::Crate(const QString &addr, QObject *parent)
  : QObject{parent}
  , address(addr)
  , ltr(new TLTR{0})
  , _modules(new QHash<int, LTRBase*>()) {

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
      result = LTR_GetListOfIPCrates(&ltr, 0, 0, 0, &entries_found, &entries_returned, zero);
      if (result == LTR_OK) {
        if (entries_found > 0) {
          TLTR_CRATE_IP_ENTRY tmp[entries_found];
          std::memset(tmp, 0, entries_found * sizeof(TLTR_CRATE_IP_ENTRY));
          result = LTR_GetListOfIPCrates(&ltr, 1, 0, 0, &entries_found, &entries_returned, tmp);
          if (result == LTR_OK && entries_found > 0) {
            for (int i = 0; i < entries_found; i++) {
              LCCrateInfo info = {tmp[i].ip_addr, tmp[i].flags, QString(tmp[i].serial_number), tmp[i].status};
              array.append(info);
            }
          }
        }
      }
    }
  }
  if (result != LTR_OK) qDebug() << LCard::getErrorString(result);
  return result;
}

int Crate::crates(QList<QString> &list) {
  TLTR ltr = {0};
  memset(&ltr, 0, sizeof(TLTR));
  INT result = LTR_Init(&ltr);
  if (result == LTR_OK) {
    result = LTR_Open(&ltr);
    if (result == LTR_OK) {
      BYTE arr[LTR_CRATES_MAX][LTR_CRATE_SERIAL_SIZE];
      result = LTR_GetCrates(&ltr, arr[0]);
      if (result == LTR_OK) {
        for (int j = 0; j < LTR_CRATES_MAX; ++j) {
          if (arr[j][0] != 0)
            list.append(QString::fromLatin1(reinterpret_cast<const char*>(arr[j])).trimmed());
        }
      }
    }
  }
  return result;
}

int Crate::cratesEx(QList<CrateInfo> &list) {
  TLTR ltr = {0};
  memset(&ltr, 0, sizeof(TLTR));
  INT result = LTR_Init(&ltr);
  if (result == LTR_OK) {
    result = LTR_Open(&ltr);
    if (result == LTR_OK) {
      DWORD crates_found = 0;
      DWORD crates_returned = 0;
      CHAR arr[LTR_CRATES_MAX][LTR_CRATE_SERIAL_SIZE];
      TLTR_CRATE_INFO info_list[LTR_CRATES_MAX];  // BYTE CrateType;      -Тип крейта -значение из en_LTR_CrateTypes
                                                  // BYTE CrateInterface; -Интерфейс подключения крейта -значение из en_LTR_CrateIface

      result = LTR_GetCratesEx(&ltr, LTR_CRATES_MAX, en_LTR_GetCratesFlags::LTR_GETCRATES_FLAGS_WORKMODE_ONLY,
        &crates_found, &crates_returned, arr, info_list);
      if (result == LTR_OK) {
        for (int j = 0; j < crates_returned; ++j) {
          if (arr[j][0] != 0) {
            TLTR_CRATE_INFO info = info_list[j];
            auto sn = QString::fromLatin1(reinterpret_cast<const char*>(arr[j])).trimmed();
            list.append(CrateInfo{sn, (LCEnums::LTR_CrateType)info.CrateType, (LCEnums::LTR_CrateIface)info.CrateInterface});
          }
        }
      }
    }
  }
  return result;
}

int Crate::crateInfo(const QString &serial, CrateInfoEx &info) {
  TLTR ltr = {0};
  memset(&ltr, 0, sizeof(TLTR));
  INT result = LTR_Init(&ltr);
  if (result == LTR_OK) {

    if (serial.length() > 0) {
      QByteArray ba = serial.toLocal8Bit();
      qstrncpy(ltr.csn, ba.constData(), sizeof(ltr.csn));
    }

    result = LTR_Open(&ltr);
    if (result == LTR_OK) {
      TLTR_CRATE_DESCR descr = {};
      result = LTR_GetCrateDescr(&ltr, LCEnums::LTR_CrateIface::LTR_CRATE_IFACE_UNKNOWN, /*nullptr*/ltr.csn, &descr, sizeof(descr));
      if (result == LTR_OK) {
        info = {
          descr.devname,
          descr.serial,
          descr.soft_ver,
          descr.brd_revision,
          descr.brd_opts,
          descr.bootloader_ver,
          descr.cpu_type,
          descr.fpga_name,
          descr.fpga_version,
          descr.crate_type_name,
          //descr.spec_info,
          descr.protocol_ver_major,
          descr.protocol_ver_minor,
          descr.slots_config_ver_major,
          descr.slots_config_ver_minor
        };
      }
    }
  }
  return result;
}

int Crate::crateStat(const QString &serial, CrateStatistic &statistic) {
  TLTR ltr = {0};
  memset(&ltr, 0, sizeof(TLTR));
  INT result = LTR_Init(&ltr);
  if (result == LTR_OK) {
    result = LTR_Open(&ltr);
    if (result == LTR_OK) {

      if (serial.length() > 0) {
        QByteArray ba = serial.toLocal8Bit();
        qstrncpy(ltr.csn, ba.constData(), sizeof(ltr.csn));
      }
      TLTR_CRATE_STATISTIC stat = {sizeof(stat), 0};
      result = LTR_GetCrateStatistic(&ltr, LCEnums::LTR_CrateIface::LTR_CRATE_IFACE_UNKNOWN,  ltr.csn, &stat, sizeof(stat));
      if (result == LTR_OK) {
        statistic = {
           stat.crate_type,
        };
      }
    }
  }
  return result;
}

bool Crate::open() {
  _modules->clear();
  result = LTR_Open(ltr);
  if (result == LTR_OK) {
    WORD mid[MODULE_MAX];
    result = LTR_GetCrateModules(ltr, mid);
    if (result == LTR_OK) {
      for (int slot = 1; slot <= MODULE_MAX; slot++) {
        auto type = mid[slot]&0xFF;
        if (type > 0) {
          auto m = module(slot, type);
          if (m) _modules->insert(slot, m);
          else qDebug() << "Отсутствует модуль обработки для слота" << slot << type;
        }
      }
    }
  } else qDebug() << lastError();
  return result == LTR_OK;
}

LTRBase* Crate::module(const int &slot, const int &type) {
  switch (type) {
  case 11:
    return new class ltr11(slot, this);
  case 114:
  default:
    break;
  }
  return nullptr;
}

bool Crate::start(LCParameters *params) {
  this->params = params;
  bool res = true;
  if (params) {
    foreach (auto key, params->keys()) {
      auto m = _modules->value(key);
      if (m) {
        // Пробуем открыть канал
        res = m->open();
        if (!res) {
          result = m->error();
          qDebug() << lastError();
          break; // ???
        }
        // Подписываемся на данные
        connect(m->base(), &LTRBase::dataReady, this, [=](LTRBase *module, const int &count, double *data) {
          emit dataReady(module, count, data);
        });
        // Стартуем опрос модуля
        res = m->start(params->value(key));
        if (!res) {
          result = m->error();
          qDebug() << lastError();
          break; // ???
        }
      } else qDebug() << "Отсутствует слот с номером:" << key;
    }
  }
  return res;
}

bool Crate::close() {
  bool res = true;
  if (params && params->count() > 0) {
    foreach (auto key, params->keys()) {
      auto m = _modules->value(key);
      if (m) {
        res = m->close();
        if (!res) {
          result = m->error();
          qDebug() << lastError();
          break;
        }
      } else qDebug() << "Отсутствует слот с номером:" << key;
    }
  }
  if (result == LTR_OK)
    result = LTR_Close(ltr);
  return result == LTR_OK;
}

bool Crate::opened() {
  result = ltr ? LTR_IsOpened(ltr) : false;
  return result == LTR_OK;
}

QString Crate::version() {
  DWORD v;
  result = LTR_GetServerVersion(ltr, &v);
  if (result == LTR_OK && v > 0)
    return QString("%1.%2.%3.%4").arg((v >> 24) & 0xFF).arg((v >> 16) & 0xFF).arg((v >> 8) & 0xFF).arg(v & 0xFF);
  return nullptr;
}
