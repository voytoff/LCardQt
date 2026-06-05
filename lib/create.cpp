#include "create.h"
#include <qdebug.h>
#pragma hdrstop
#include <conio.h>
#include <winsock2.h> // Важно подключать перед ltrapi для работы сетевых типов
#include "ltrapi.h"
#pragma argsused

#include <QHostAddress>
#include <QString>

Create::Create(QObject *parent) : QObject{parent} {}

void Create::getModuleList() {
  INT res;
  TLTR ltr;
  res=LTR_Init(&ltr);
  if(res==LTR_OK) {
    ltr.cc=CC_CONTROL;
    res=LTR_Open(&ltr);
    if(res==LTR_OK) {
      WORD mid[MODULE_MAX];
      res=LTR_GetCrateModules(&ltr, mid);
      if(res==LTR_OK) {
        qDebug() << "список установленных модулей";
        for(int i=0; i<MODULE_MAX; i++)
          qDebug() << QString(">> slot%1  mid=%2").arg(i+1).arg(mid[i]&0xFF);
      }
    }
  }
  qDebug() << QString(decoder.decode(LTR_GetErrorString(res)));
}


void Create::getIPAddresses() {
  INT res;
  TLTR ltr;
  qstrncpy(ltr.csn, "#SERVER_CONTROL", 16);
  //ltr.csn = "#SERVER_CONTROL";

  DWORD entries_found = 0;
  DWORD entries_returned = 0;
  TLTR_CRATE_IP_ENTRY *zero = nullptr;
  res=LTR_Init(&ltr);
  if(res==LTR_OK) {
    ltr.cc=CC_CONTROL;
    res=LTR_Open(&ltr);
    if(res==LTR_OK) {
      res = LTR_GetListOfIPCrates(&ltr, 0, 0, 0, &entries_found, &entries_returned, zero);
      if (res == LTR_OK && entries_found > 0) {
        TLTR_CRATE_IP_ENTRY array[entries_found];
        std::memset(array, 0, entries_found * sizeof(TLTR_CRATE_IP_ENTRY));
        res = LTR_GetListOfIPCrates(&ltr, 0, 0, 0, &entries_found, &entries_returned, array);
        if (res == LTR_OK && entries_found > 0) {
          for (int i = 0; i < entries_found; i++) {
            qDebug() << array[i].flags << QHostAddress(array[i].ip_addr).toString() << array[i].is_dynamic << array[i].serial_number << array[i].status;
          }
        } else {
        }
      } else {
        qDebug() << QString(decoder.decode(LTR_GetErrorString(res)));
      }
    }
  }
}
