#ifndef CREATE_H
#define CREATE_H

#include "LCard_global.h"
#include <QObject>
#include <QStringDecoder>

class LCARD_EXPORT Create : public QObject
{
  Q_OBJECT
public:
  explicit Create(QObject *parent = nullptr);

public slots:
  void getModuleList();
  void getIPAddresses();

signals:
};

#endif // CREATE_H
