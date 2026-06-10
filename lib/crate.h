#ifndef CREATE_H
#define CREATE_H

#include "lcard.h"
#pragma hdrstop
#include <conio.h>
#include <winsock2.h> // Важно подключать перед ltrapi для работы сетевых типов
#pragma argsused
#include "ltrapi.h"

#include "LCard_global.h"
#include "lctypes.h"
#include "ilcmodule.h"

#include <QObject>
#include <QStringDecoder>
#include <QList>
#include <QHash>

class LCARD_EXPORT Crate : public QObject {
  Q_OBJECT
public:
  explicit Crate(const QString &addr = nullptr, QObject *parent = nullptr);

private:
  QString address;
  TLTR *ltr;
  QString ver;
  INT result;
  /** загружает хеш таблицу модулей */
  QHash<int, ILCModule*> *modules;
  LCParameters *params;

protected:
  bool init();
  ILCModule *module(const int &slot, const int &type);

public slots:
  static int addresses(QList<LCCrateInfo> &array);
  bool open();
  bool close();
  bool start(LCParameters *params);
  bool stop();
  bool opened();
  uint version();
  INT error() const {return result;}
  QString lastError() const {return LCard::getErrorString(result);}
  QString &ip() {return address;}
  /** возвращает информацию о модуле крейта
   *  key = номер слота
   */
  QHash<int, ILCModule*> &hardware() const {return *modules;}

signals:
  void dataReady(ILCModule *module, const int &count, double *data);

};

#endif // CREATE_H
