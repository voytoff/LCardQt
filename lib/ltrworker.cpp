#include "ltrworker.h"
#include "lctypes.h"
#include "ltr11.h"

LTRWorker::LTRWorker(LTRBase *module, TLTR11 *ltr, const int &dataBuferLength, double *data, QObject *parent)
  : QObject{parent}
  , module(module)
  , ltr(ltr)
  , dataBuferLength(dataBuferLength)
  , data(data) {
}

void LTRWorker::doWork() {
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
      emit dataReady(module, ltr->LChQnt, data);

  } //while (!f_out && (err==LTR_OK))
  free(rbuf);
}

