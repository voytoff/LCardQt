#ifndef LCTYPES_H
#define LCTYPES_H

#include "ltr11api.h"
#include "lcenums.h"

#include <QString>
#include <QHostAddress>
#include <QDateTime>
#include <QMap>

/* Количество отсчетов на канал, принмаемых за раз */
#define RECV_BLOCK_CH_SIZE  4096*8
/* таймаут на ожидание данных при приеме (без учета времени преобразования) */
#define RECV_TOUT  4000

const int OK = LTR_OK;

struct ThreadState {
  bool running = false;
  long count = 0;
};

struct CrateInfo {
  QString sn;
  LCEnums::LTR_CrateType type;    // Тип крейта
  LCEnums::LTR_CrateIface iface;  // Интерфейс подключения крейта
};

struct CrateInfoEx {
  QString devname;                // Название крейта
  QString serial;                 // Серийный номер
  QString softVer;                // Версия прошивки
  QString brdRevision;            // Ревизия платы
  QString brdOpts;                // Опции платы
  QString bootloaderVer;          // Версия загрузчика
  QString cpuType;                // Тип микроконтроллера
  QString fpgaName;               // Название ПЛИС в крейте
  QString fpgaVersion;            // Версия прошивки ПЛИС
  QString crateTypeName;          // Строка с типом крейта
  BYTE protocolVerMajor;          // Версия протокола между ltrd и крейтом (мажорная)
  BYTE protocolVerMinor;          // Версия протокола между ltrd и крейтом (минорная)
  BYTE slotsConfigVerMajor;       // Версия протокола для сохранения настроек модуля (мажорная)
  BYTE slotsConfigVerMinor;       // Версия протокола для сохранения настроек модуля (минорная)
};

struct CrateStatistic {
  LCEnums::LTR_CrateType crateType;       // Тип крейта из #en_LTR_CrateTypes
  LCEnums::LTR_CrateIface crateIntf;      // Интерфейс, по которому подключен крейт из #en_LTR_CrateIface
  LCEnums::LTR_CrateMode crateMode;       // Режим работы крейта из #en_LTR_CrateMode
  ULONGLONG conTime;                      // Время установления соединения службы с крейтом (формат unixtime)
  WORD modulesCnt;                        // Количество слотов в данном типе крейта
  WORD mids[LTR_MODULES_PER_CRATE_MAX];   // ID-модулей для всех слотов крейта
  WORD ctlClientsCnt;                     // Количество клиентов, подключенных по управляющему каналу к крейту
  WORD totalModClientsCnt;                // Количество клиентов, подключенных ко всем модулям крейта
  ULONGLONG wrdSent;                      // Общее количество слов, переданных в крейт (крейту и всем его модулям)
  ULONGLONG wrdRecv;                      // Общее количество слов, принятое от крейта (от самого крейта и всех его модулей)
  double bwSend;                          // Текущая скорость передачи слов в крейт (слов/с)
  double bwRecv;                          // Текущая скорость приема слов из крейта (слов/c)
  ULONGLONG crateWrdRecv;                 // Количество принятых слов непосредственно от крейта
  ULONGLONG internalrBufMiss;             // Количество потерянных буферов в крейте из-за внутреннего переполнения
  DWORD internalrBufOvfls;                // Количество переполнений внутреннго буфера крейта
  DWORD rBufOvfls;                        // Количество переполнений буфера прима данных от модулей в службе ltrd для модулей крейта (суммарное по всем модулям)
  DWORD totalStartMarks;                  // Количество принятых меток "Старт" как от крейта, так и от модулей
  DWORD totalSecMarks;                    // Количество принятых секундных меток как от крейта, так и от модулей
  DWORD crateStartMarks;                  // Количество принятых меток "Старт" непосредственно от крейта
  DWORD crateSecMarks;                    // Количество принятых секундных меток непосредственно от крейта
  ULONGLONG crateUnixtime;                // Последнее значение расширенной секундной метки (формат unixtime), если поддерживается крейтом
  DWORD thermMask;                        // Маска действительных показаний термометров (если не поддерживается --- 0)
  float thermVals[LTR_CRATE_THERM_MAX_CNT]; // значение показаний термометров крейта. действительны только если соответствующий бит в therm_mask в 1

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
