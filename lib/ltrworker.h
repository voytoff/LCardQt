#ifndef LTRWORKER_H
#define LTRWORKER_H

#include "ltr11api.h"
#include "ltrbase.h"

#include <QObject>
#include <QDebug>

class LTRWorker : public QObject {
  Q_OBJECT
public:
  explicit LTRWorker(LTRBase *module, TLTR11 *ltr, const int &dataBuferLength, double *data, QObject *parent = nullptr);
  explicit LTRWorker(const int &dataBuferLength, double *data, const std::function<void()> callback, QObject *parent = nullptr);
  virtual void doWork();

  INT result;

private:
  LTRBase *module;
  TLTR11 *ltr;
  int dataBuferLength;
  double *data;
  const std::function<void()> callback = nullptr;

signals:
  void finished();
  void dataReady(LTRBase *module, const int &count, double *data);

};

#endif // LTRWORKER_H
