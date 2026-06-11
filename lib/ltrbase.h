#ifndef LTRBASE_H
#define LTRBASE_H

#include "lctypes.h"
#include "lcard.h"
#include <QObject>

class LTRWorker;
class LTRBase : public QObject
{
  Q_OBJECT
public:
  LTRBase(QObject *parent = nullptr);
  virtual bool open(const int &slot, const QString &serial = nullptr) = 0;
  virtual bool opened() = 0;
  virtual bool start(void *param) = 0;
  virtual bool close() = 0;
  virtual LCModuleInfo* info() = 0;
  virtual LTRBase* base() const = 0;

  INT error() const {return result;}
  QString lastError() const {return LCard::getErrorString(result);}
  LTRWorker *worker = nullptr;
  ThreadState state;

protected:
  INT result;
  int slot = 0;
  double *data = nullptr;

signals:
  void dataReady(LTRBase *module, const int &count, double *data);

};

#endif // LTRBASE_H
