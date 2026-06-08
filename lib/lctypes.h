#ifndef LCTYPES_H
#define LCTYPES_H

#include "ltr11api.h"

#include <QString>
#include <QHostAddress>
#include <QDateTime>

struct LCCrateInfo {
  ulong ipAddress;        // IP-адрес крейта. Формат аналогичен полю saddr в #TLTR
  QString strAddress() const {return QHostAddress(ipAddress).toString();}
  ulong flags;            // Набор флагов, связанных с данной записью, из #en_LTR_CrateIpFlags
  QString serialNumber;   // Если крейт подключен, то в данном поле содержится серийный номер
                          // подключенного крейта. Этот номер можно использовать для
                          // открытия соединения с крейтом. Для остальных значений status данное
                          // поле содержит пустую строку, т.к. серийный номер неизвестен
  short status;           //  Состояние подключения крейта, соответствующего данной записи. Одно значение из #en_LTR_CrateIpStatus
};

struct  LCModuleInfo {
  int type;
  int slot;
  QString name;       // название модуля (строка)
  QString serial;     // серийный номер модуля (строка)
  int version;        // версия ПО модуля (младший байт - минорная, старший - мажорная)
  QDateTime date;     // дата создания ПО (строка)
  struct {
    double offset;    // смещение нуля
    double gain;      // масштабный коэффициент
  } calibration[LTR11_ADC_RANGEQNT]; // калибровочные коэффициенты для каждого диапазона
};

#endif // LCTYPES_H
