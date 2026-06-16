#include "ltr11.h"
#include "lctypes.h"
#include "ltrworker.h"
#include "worker.h"

#include <QHostAddress>
#include <QDateTime>
#include <QString>
#include <QFuture>
#include <QtConcurrent>

ltr11::ltr11(const int &slot, QObject *parent)
  : LTRBase(slot, parent)
  , ltr(new TLTR11{}) {
  /* инициализация дескриптора модуля */
  LTR11_Init(ltr);
}

bool ltr11::open(const QString &serial) {
  if (opened()) return LTR_OK;
  /* открытие канала связи с модулем, установленным в заданный слот */
  result = LTR11_Open(ltr, QHostAddress(crate()->ip()).toIPv4Address(), LTRD_PORT_DEFAULT, serial.toUtf8().data(), slot);
  if (result == LTR_OK)
    /* получение информации о модуле из flash-памяти */
    result = LTR11_GetConfig(ltr);
  return result == LTR_OK;
}

bool ltr11::opened() {
  result = LTR11_IsOpened(ltr);
  qDebug() << lastError();
  return result == LTR_OK;
}

bool ltr11::close() {
  if (opened()) {
    state.running = false;
    pause(500);
    //delete worker;
    worker = nullptr;

    result = LTR11_Stop(ltr);
    if (result == LTR_OK)
      result = LTR11_Close(ltr);
    free(data);
    data = nullptr;
  }
  return result == LTR_OK;
}

LCModuleInfo* ltr11::info() {
  auto i = ltr->ModuleInfo;
  QString format = "(MMM d yyyy)";
  LCModuleInfo *info = new LCModuleInfo {11, slot, i.Name, i.Serial, i.Ver, QDateTime::fromString(i.Date, format)};
  return info;
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
  foreach (auto key,  p->channels.keys()) {
    auto c = p->channels.value(key);
    ltr->LChTbl[key] = LTR11_CreateLChannel(c.phy_ch, c.mode, c.range);
  }
  ltr->ADCMode = p->adcMode;
  /* частота дискретизации - 400 кГц. Подбираем нужные делители */
  result = LTR11_FindAdcFreqParams(400000, &ltr->ADCRate.prescaler, &ltr->ADCRate.divider, NULL);
  if (result != LTR_OK) return false;
  /* передаем настройки в модуль */
  result = LTR11_SetADC(ltr);
  if (result != LTR_OK) return false;

  INT recv_data_cnt = RECV_BLOCK_CH_SIZE * ltr->LChQnt;         // 4096*8*LChQnt
  data = (double *)malloc(recv_data_cnt*sizeof(double));

  if (data == NULL) {
    free(data);
    result = LTR_ERROR_MEMORY_ALLOC;
    return false;
  }
/*
  worker = new LTRWorker([=]() {dataThreadFunction(recv_data_cnt, data);});
  connect(worker, &LTRWorker::finished, this, &LTRBase::finished);
  connect(worker, &LTRWorker::dataReady, this, &LTRBase::dataReady);

  state.running = true;
  result = LTR11_Start(ltr);

  // запуск потока
  worker->start();
*/
/*
WorkerPool::instance().run([]()
{
    qDebug() << "Worker thread:"
             << QThread::currentThread();
});
*/

  state.running = true;
  result = LTR11_Start(ltr);
  worker = new Worker();
  connect(this, &LTRBase::finished, worker, &Worker::finished);
  worker->post([recv_data_cnt, this]() -> QString {
    dataThreadFunction(recv_data_cnt, data);
    return "ОК";
  });
  return result == LTR_OK;
}

void ltr11::dataThreadFunction(const int &dataBuferLength, double *data) {
  DWORD *rbuf = (DWORD*)malloc(dataBuferLength*sizeof(DWORD));
  INT recvd;
  while (state.running) {
    /* в таймауте учитываем время выполнения самого преобразования*/
    DWORD tout = RECV_TOUT + (DWORD)(RECV_BLOCK_CH_SIZE/ltr->ChRate + 1);
    /* получение данных от LTR11 */
    recvd = LTR11_Recv(ltr, rbuf, NULL, dataBuferLength, tout);
    if (!state.running) break;
    /* Значение меньше нуля соответствуют коду ошибки */
    if (recvd < 0) {
      result = recvd;
      break;
    } else if (recvd != dataBuferLength) {
      result = LTR_ERROR_RECV_INSUFFICIENT_DATA;
      continue;
    }
    /* Cохранение принятых и обработанных данных в буфере */
    result = LTR11_ProcessData(ltr, rbuf, data, &recvd, TRUE, TRUE);
    if (!state.running) break;
    if (result != LTR_OK) continue;
    else if (state.running)
      emit dataReady(this, ltr->LChQnt, data);

    state.count++;
    qDebug() << state.count;

  } // while (...)
  free(rbuf);
  emit finished();
  qDebug() << "dataThreadFunction ended !!!";
}

