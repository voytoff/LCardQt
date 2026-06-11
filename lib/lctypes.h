#ifndef LCTYPES_H
#define LCTYPES_H

#include "ltr11api.h"

#include <QString>
#include <QHostAddress>
#include <QDateTime>
#include <QMap>

/* Количество отсчетов на канал, принмаемых за раз */
#define RECV_BLOCK_CH_SIZE  4096*8
/* таймаут на ожидание данных при приеме (без учета времени преобразования) */
#define RECV_TOUT  4000

struct ThreadState {
  bool running = false;
  long count = 0;
};

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

struct LCModuleInfo {
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

typedef QHash<int, void*> LCParameters;

struct LCChannel11 {
  BYTE phy_ch;
  BYTE mode;
  BYTE range;
};

typedef QMap<int, LCChannel11> LCChannels11;

struct LCParam11 {
  LCParam11(INT startADCMode, INT inpMode, INT adcMode, LCChannels11 channels) {
    this->startADCMode = startADCMode;
    this->inpMode = inpMode;
    this->adcMode = adcMode;
    this->channels = channels;
  }
  INT startADCMode; /** режим начала сбора данных:
                     * LTR11_STARTADCMODE_INT     - внутренний старт (по команде хоста);
                     * LTR11_STARTADCMODE_EXTRISE - по фронту внешнего сигнала;
                     * LTR11_STARTADCMODE_EXTFALL - по спаду внешнего сигнала.
                     */
  INT inpMode;      /** режим ввода данных с АЦП
                     *  LTR11_INPMODE_INT     - внутренний запуск АЦП (частота задается AdcRate)
                     *  LTR11_INPMODE_EXTRISE - по фронту внешнего сигнала
                     *  LTR11_INPMODE_EXTFALL - по спаду внешнего сигнала
                     */
  INT adcMode;      /** режим сбора данных или тип тестового режима */
  LCChannels11 channels;  /** таблица управления логическими каналами */
  int lChQnt() const {return channels.count();} /* число активных логических каналов (размер кадра) */
};

#endif // LCTYPES_H
