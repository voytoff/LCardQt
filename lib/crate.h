#ifndef CREATE_H
#define CREATE_H

#pragma hdrstop
#include <conio.h>
#include <winsock2.h> // Важно подключать перед ltrapi для работы сетевых типов
#pragma argsused
#include "ltrapi.h"

#include "LCard_global.h"
#include "lctypes.h"

#include <QObject>
#include <QStringDecoder>
#include <QList>
#include <QHash>

class LCARD_EXPORT Crate : public QObject {
  Q_OBJECT
public:
  explicit Crate(QObject *parent = nullptr);

private:
  TLTR *ltr;
  QString address;
  QHash<int, LCModuleInfo*> *hardware;

public slots:
  static int addresses(QList<LCCrateInfo> &array);
  bool open(const QString &addr);
  bool close();
  bool start();
  bool stop();
  bool isOpened();
  uint version();
  LCModuleInfo *info(const int &slot, const int &type);
  QList<LCModuleInfo*> modules();

signals:

};

#endif // CREATE_H
