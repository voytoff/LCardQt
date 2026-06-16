#ifndef WORKER_H
#define WORKER_H

#include "ltrbase.h"
#include <QObject>
#include <QThread>

class Worker : public QObject {
  Q_OBJECT

public:
  explicit Worker(QObject* parent = nullptr);
  ~Worker();

  void deleteLater();

  template<typename F>
  void post(F&& f) {
    QMetaObject::invokeMethod(this, std::forward<F>(f), Qt::QueuedConnection);
  }

public slots:
  void doWork();

signals:
  void finished();
  void dataReady(LTRBase *module, const int &count, double *data);

private:
  QThread *thread;
  const std::function<void()> callback = nullptr;
};

#endif // WORKER_H
