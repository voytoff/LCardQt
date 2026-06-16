#include "ltrworker.h"
#include "lctypes.h"
#include "QThread"

LTRWorker::LTRWorker(LTRBase *module, TLTR11 *ltr, const int &dataBuferLength, double *data, QObject *parent)
  : QObject{parent}
  , module(module)
  , ltr(ltr)
  , dataBuferLength(dataBuferLength)
  , data(data)
  , callback() {
}

LTRWorker::LTRWorker(const std::function<void ()> callback, QObject *parent)
  : QObject{parent}
  , callback(callback)
  , thread(new QThread(this)) {
  connect(thread, &QThread::finished, thread, &QObject::deleteLater);
  connect(thread, &QThread::started, this, &LTRWorker::doWork);
  connect(thread, &QThread::finished, this, &QObject::deleteLater);
  connect(this, &LTRWorker::finished, thread, &QThread::quit);//, Qt::DirectConnection);
  connect(this, &LTRWorker::finished, this, &LTRWorker::deleteLater);
  connect(this, &LTRWorker::finished, this, &QObject::deleteLater);
  this->moveToThread(thread);
}

LTRWorker::~LTRWorker() {
  if (thread->isRunning()) {
    thread->quit();
    thread->wait(); // Жестко ждем завершения перед уничтожением
  }
}

void LTRWorker::doWork() {
  if (callback) callback();
  else {
    DWORD *rbuf = (DWORD*)malloc(dataBuferLength*sizeof(DWORD));
    while (true) {
      INT recvd;
      /* в таймауте учитываем время выполнения самого преобразования*/
      DWORD tout = RECV_TOUT + (DWORD)(RECV_BLOCK_CH_SIZE/ltr->ChRate + 1);
      /* получение данных от LTR11 */
      recvd = LTR11_Recv(ltr, rbuf, NULL, dataBuferLength, tout);

      /* Значение меньше нуля соответствуют коду ошибки */
      if (recvd < 0) {
        result = recvd;
        break;
      } else if (recvd != dataBuferLength) {
        result = LTR_ERROR_RECV_INSUFFICIENT_DATA;
        continue;
      }

      /* сохранение принятых и обработанных данных в буфере */
      result = LTR11_ProcessData(ltr, rbuf, data, &recvd, TRUE, TRUE);
      if (result != LTR_OK) continue;
      else
        emit this->dataReady(module, ltr->LChQnt, data);

    } //while (...)
    free(rbuf);
  }
  emit finished();
  thread->quit();
}

void LTRWorker::start() {
  if (thread && !thread->isRunning())
    thread->start();
}

void LTRWorker::quit() {
  if (thread) thread->quit();
}

void LTRWorker::terminate() {
  if (thread) thread->terminate();
}

bool LTRWorker::wait(unsigned long time) {
  return thread ? thread->wait(time) : true;
}

