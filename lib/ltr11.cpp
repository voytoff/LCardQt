#include "ltr11.h"
#include "lctypes.h"
#include <QHostAddress>
#include <QDateTime>
#include <QString>

ltr11::ltr11(QObject *parent)
  : QObject{parent}
  , ltr(new TLTR11{})
  , slot(0) {
  /* инициализация дескриптора модуля */
  LTR11_Init(ltr);
}

bool ltr11::open(const int &slot, const QString &serial) {
  if (opened()) return LTR_OK;
  this->slot = slot;
  /* открытие канала связи с модулем, установленным в заданный слот */
  result = LTR11_Open(ltr, QHostAddress(crate()->ip()).toIPv4Address(), LTRD_PORT_DEFAULT, serial.toUtf8().data(), slot);
  if (result == LTR_OK)
    /* получение информации о модуле из flash-памяти */
    result = LTR11_GetConfig(ltr);
  return result == LTR_OK;
}

bool ltr11::opened() {
  result = LTR11_IsOpened(ltr);
  return result == LTR_OK;
}

bool ltr11::start(void* param) {
  LCParam11 *p = (LCParam11*)param;

  /* задание параметров работы модуля */
  /* режим старта сбора данных */
  ltr->StartADCMode = p->startADCMode;
  /* режим синхронизации АПЦ */
  ltr->InpMode = p->inpMode;
  /* количество логических каналов */
  ltr->LChQnt = p->channels.count();
  /* таблица управления логическими каналами */
  foreach (auto c,  p->channels) {
    ltr->LChTbl[c.phy_ch] = LTR11_CreateLChannel(c.phy_ch, c.mode, c.range);
  }
  ltr->ADCMode = p->adcMode;
  /* частота дискретизации - 400 кГц. Подбираем нужные делители */
  result = LTR11_FindAdcFreqParams(400000, &ltr->ADCRate.prescaler, &ltr->ADCRate.divider, NULL);
  if (result != LTR_OK) return false;
  /* передаем настройки в модуль */
  result = LTR11_SetADC(ltr);
  if (result != LTR_OK) return false;

  DWORD recvd_blocks = 0;
  INT recv_data_cnt = RECV_BLOCK_CH_SIZE * ltr->LChQnt;
  DWORD *rbuf = (DWORD*)malloc(recv_data_cnt*sizeof(rbuf[0]));
  double *data = (double *)malloc(recv_data_cnt*sizeof(data[0]));

  if (rbuf == NULL || data == NULL) {
    free(rbuf);
    free(data);
    result = LTR_ERROR_MEMORY_ALLOC;
    return false;
  }

  result = LTR11_Start(ltr);

  f_out = 0;
  while (!f_out && (result==LTR_OK)) {
    INT recvd;
    /* в таймауте учитываем время выполнения самого преобразования*/
    DWORD tout = RECV_TOUT + (DWORD)(RECV_BLOCK_CH_SIZE/ltr->ChRate + 1);
    /* получение данных от LTR11 */
    recvd = LTR11_Recv(ltr, rbuf, NULL, recv_data_cnt, tout);
    /* Значение меньше нуля соответствуют коду ошибки */
    if (recvd<0) {
      result = recvd;
      qDebug() << lastError();
    } else if (recvd!=recv_data_cnt) {
      result = LTR_ERROR_RECV_INSUFFICIENT_DATA;
      qDebug() << lastError();
    } else {
      /* сохранение принятых и обработанных данных в буфере */
      result = LTR11_ProcessData(ltr, rbuf, data, &recvd, TRUE, TRUE);
      if (result != LTR_OK) {
        qDebug() << lastError();
      } else {
        INT ch;
        recvd_blocks++;
        QDebug dbg = qDebug();
        /* выводим по первому слову на канал */
        dbg << "Блок: " << recvd_blocks;

        for (ch=0; ch < ltr->LChQnt; ch++) {
          dbg << data[ch];
          if (ch == (ltr->LChQnt-1)) {
            dbg << "\n";
          } else {
            dbg << ", ";
          }
        }
      }
    }
#ifdef _WIN32
    /* проверка нажатия клавиши для выхода */
    if (result == LTR_OK) {
      if (_kbhit())
        f_out = 1;
    }
#endif
  } //while (!f_out && (err==LTR_OK))


  free(rbuf);
  free(data);
  return result == LTR_OK;
}

bool ltr11::stop() {
  result = LTR11_Stop(ltr);
  return result == LTR_OK;
}

bool ltr11::close() {
  if (opened()) result = LTR11_Close(ltr);
  return result == LTR_OK;
}

LCModuleInfo* ltr11::info() {
  auto i = ltr->ModuleInfo;
  QString format = "(MMM d yyyy)";
  LCModuleInfo *info = new LCModuleInfo {11, slot, i.Name, i.Serial, i.Ver, QDateTime::fromString(i.Date, format)};
  return info;
}

