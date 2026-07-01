#ifndef LCARD_H
#define LCARD_H

#include "LCard_global.h"

class LCARD_EXPORT LCard
{
public:
  LCard();
  static QString getErrorString(int res);
};

#endif // LCARD_H

/*
LTR_Init(TLTR *hnd);                                              - Инициализация описателя соединения
LTR_Open(TLTR *hnd);                                              - Открытие соединения
LTR_OpenSvcControl(TLTR *hsrv, DWORD ltrd_addr, WORD ltrd_port);  - Открытие управляющего соединения со службой ltrd
LTR_OpenCrate(TLTR *hcrate, DWORD ltrd_addr, WORD ltrd_port,      - Открытие управляющего соединения с крейтом
  INT crate_iface, const char *crate_sn);
LTR_OpenEx(TLTR *hnd, DWORD timeout);                             - Открытие соединения с заданным таймаутом
LTR_Close(TLTR *hnd);                                             - Закрытие соединения
LTR_IsOpened(TLTR *hnd);                                          - Проверка, открыто ли соединение

LTR_GetServerVersion(TLTR *hsrv, DWORD *version);                 - Получение версии службы ltrd
LTR_GetCrates(TLTR *hsrv, BYTE *csn);                             - Получение серийных номеров подключенных крейтов
LTR_GetCratesEx(TLTR *hsrv, DWORD max_crates, DWORD flags,        - Получение информации о подключенных крейтах
  DWORD *crates_found, DWORD *crates_returned,
  CHAR serials[][LTR_CRATE_SERIAL_SIZE],
  TLTR_CRATE_INFO *info_list);
LTR_GetCrateDescr(TLTR *hsrv, INT crate_iface,                    - Получение описания крейта
  const char *crate_sn, TLTR_CRATE_DESCR *descr, DWORD size);
LTR_GetCrateStatistic(TLTR *hsrv, INT crate_iface,                - Получение статистики по крейту
  const char *crate_sn,
  TLTR_CRATE_STATISTIC *stat, DWORD size);
LTR_GetModuleStatistic(TLTR *hsrv, INT crate_iface,               - Получение статистики по модулю
  const char *crate_sn, INT module_slot,
  TLTR_MODULE_STATISTIC *stat, DWORD size);

LTR_GetCrateModules(TLTR *hcrate, WORD *mid);                     - Получение списка модулей в крейте
LTR_GetCrateInfo(TLTR *hcrate, TLTR_CRATE_INFO *CrateInfo);       - Получение информации о типе и интерфейсе подключения крейта
LTR_Config(TLTR *hcrate, const TLTR_CONFIG *conf);                - Конфигурация линий разъема синхронизации крейта
LTR_MakeStartMark(TLTR *hcrate, INT mode);                        - Настройка генерации метки "СТАРТ"
LTR_StartSecondMark(TLTR *hcrate, INT mode);                      - Запуск генерации меток "СЕКУНДА"
LTR_StopSecondMark(TLTR *hcrate);                                 - Останов генерации меток "СЕКУНДА"

LTR_ResetModule(TLTR *hsrv, INT crate_iface,                      - Сброс указанного модуля.
  const char *crate_sn, INT module_slot,
  DWORD flags);
LTR_SetLogLevel(TLTR *hsrv, INT level, BOOL permanent);           - Установка уровня журнала
LTR_GetLogLevel(TLTR *hsrv, INT *level);                          - Получение текущего уровня журнала
LTR_SetServerParameter(TLTR *hsrv, DWORD param, void *val,        - Установка параметров работы службы ltrd
  DWORD size);
LTR_GetServerParameter(TLTR *hsrv, DWORD param, void *val,        - Чтение параметров работы службы ltrd
  DWORD *size);
LTR_ServerRestart(TLTR *hsrv);                                    - Перезапуск службы ltrd
LTR_ServerShutdown(TLTR *hsrv);                                   - Останов службы ltrd

LTR_GetListOfIPCrates(TLTR *hsrv, DWORD max_entries, DWORD ip_net,- Получение списка записей с IP-адресами крейтов из настроек службы ltrd
  DWORD ip_mask, DWORD *entries_found, DWORD *entries_returned,
  TLTR_CRATE_IP_ENTRY *info_array);
LTR_AddIPCrate(TLTR *hsrv, DWORD ip_addr, DWORD flags,            - Добавление записи с IP-адресом крейта
  BOOL permanent);
LTR_DeleteIPCrate(TLTR *hsrv, DWORD ip_addr, BOOL permanent);     - Удаление записи с IP-адресом крейта
LTR_ConnectIPCrate(TLTR *hsrv, DWORD ip_addr);                    - Установление соединения с крейтом по IP-адресу
LTR_DisconnectIPCrate(TLTR *hsrv, DWORD ip_addr);                 - Разрыв соединения с крейтом по IP-адресу
LTR_ConnectAllAutoIPCrates(TLTR *hsrv);                           - Установление соединения со всеми крейтами с признаком автоподключения
LTR_DisconnectAllIPCrates(TLTR *hsrv);                            - Разрыв соединения со всеми крейтами, подключенными по Ethernet
LTR_SetIPCrateFlags(TLTR *hsrv, DWORD ip_addr, DWORD flags,       - Установка флагов для записи с IP-адресом крейта
  BOOL permanent);
LTR_Recv(TLTR *hmodule, DWORD *data, DWORD *tmark, DWORD size,    - Прием данных от модуля
  DWORD timeout);
LTR_Send(TLTR *hmodule, const DWORD *data, DWORD size,            - Передача данных в модуль
  DWORD timeout);
LTR_GetLastUnixTimeMark(TLTR *hmodule, LONGLONG *unixtime);       - Чтение времени последней секундной метки
LTR_RecvEx(TLTR *ltr, DWORD *data, DWORD *tmark, DWORD size,      - Прием данных из крейта с чтением массива меток времени в формате 64-бит unixtime
  DWORD timeout, LONGLONG *unixtime);

LTR_GetErrorString(INT err);                                      - Получение текстового сообщения об ошибке
LTR_SetTimeout(TLTR *hnd, DWORD tout);                            - Установка таймаута по умолчанию для соединения

*/