#ifndef LTRBASE_H
#define LTRBASE_H

#include "ilcmodule.h"
#include <QObject>

class LTRBase : public QObject
{
  Q_OBJECT
public:
  //virtual LCModuleInfo* info() = 0;
  virtual bool open(const int &slot, const QString &serial = nullptr) = 0;
  virtual bool opened() = 0;
  virtual bool start(void *param) = 0;
  virtual bool stop() = 0;
  virtual INT error() const = 0;
  virtual LTRBase* base() const = 0;

signals:
  void dataReady(ILCModule *module, const int &count, double *data);

};

#endif // LTRBASE_H
