#include "ltr11.h"
#include <QHostAddress>
#include <QDateTime>
#include <QString>

ltr11::ltr11(QObject *parent)
  : QObject{parent}
  , hltr11(new TLTR11{}) {
  /* инициализация дескриптора модуля */
  LTR11_Init(hltr11);
}

bool ltr11::open(const QString &addr, int slot, QString serial) {
  /* открытие канала связи с модулем, установленным в заданный слот */
  INT res = LTR11_Open(hltr11, QHostAddress(addr).toIPv4Address(), LTRD_PORT_DEFAULT, serial.toUtf8().data(), slot);
  if (res == LTR_OK)
    /* получение информации о модуле из flash-памяти */
    res = LTR11_GetConfig(hltr11);
  return res == LTR_OK;
}

LCModuleInfo* ltr11::info() {
  auto i = hltr11->ModuleInfo;
  LCModuleInfo *info = new LCModuleInfo {11, i.Name,i.Serial, i.Ver, QDateTime::fromString(i.Date)};
  return info;
}

