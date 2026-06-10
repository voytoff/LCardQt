#include "ltrworker.h"

LTRWorker::LTRWorker(TLTR11 *module, const int &dataBuferLength, double *data, QObject *parent)
  : QObject{parent}
  , module(module)
  , dataBuferLength(dataBuferLength)
  , data(data) {

  DWORD *rbuf = (DWORD*)malloc(dataBuferLength*sizeof(DWORD));
  while (true) {
    INT recvd;
    /* в таймауте учитываем время выполнения самого преобразования*/
    DWORD tout = RECV_TOUT + (DWORD)(RECV_BLOCK_CH_SIZE/module->ChRate + 1);
    /* получение данных от LTR11 */
    recvd = LTR11_Recv(module, rbuf, NULL, dataBuferLength, tout);

    /* Значение меньше нуля соответствуют коду ошибки */
    if (recvd < 0) {
      result = recvd;
      break;
    } else if (recvd != dataBuferLength) {
      result = LTR_ERROR_RECV_INSUFFICIENT_DATA;
      continue;
    }

    /* сохранение принятых и обработанных данных в буфере */
    result = LTR11_ProcessData(module, rbuf, data, &recvd, TRUE, TRUE);
    if (result != LTR_OK) continue;
    else
      emit dataReady((ILCModule*)module, module->LChQnt, data);

  } //while (!f_out && (err==LTR_OK))
  free(rbuf);

}
