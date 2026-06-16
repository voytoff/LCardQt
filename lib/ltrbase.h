#ifndef LTRBASE_H
#define LTRBASE_H

#include "lctypes.h"
#include "lcard.h"
#include <QObject>

class Worker;
class LTRBase : public QObject {
  Q_OBJECT
public:
  LTRBase(const int &slot, QObject *parent = nullptr);
  virtual bool open(const QString &serial = nullptr) = 0;
  virtual bool opened() = 0;
  virtual bool start(void *param) = 0;
  virtual bool close() = 0;
  virtual LCModuleInfo* info() = 0;
  virtual LTRBase* base() const = 0;

  INT error() const {return result;}
  QString lastError() const {return LCard::getErrorString(result);}
  Worker *worker = nullptr;
  ThreadState state;
  int slot = 0;

protected:
  INT result;
  double *data = nullptr;
  void pause(int msec);

signals:
  void finished();
  void dataReady(LTRBase *module, const int &count, double *data);

};

#endif // LTRBASE_H
