#ifndef LTRWORKER_H
#define LTRWORKER_H

#include "ilcmodule.h"
#include <QObject>
#include <QDebug>

class LTRWorker : public QObject {
  Q_OBJECT
public:
  explicit LTRWorker(TLTR11 *module, const int &dataBuferLength, double *data, QObject *parent = nullptr);
  virtual void doWork() = 0;

  INT result;

private:
  TLTR11 *module;
  int dataBuferLength;
  double *data;

signals:
  void finished();
  void dataReady(ILCModule *module, const int &count, double *data);

};

#endif // LTRWORKER_H
