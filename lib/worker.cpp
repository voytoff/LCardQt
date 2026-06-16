#include "worker.h"

Worker::Worker(QObject* parent)
  : QObject(nullptr)
  , thread (new QThread(this)) {
  moveToThread(thread);

  //connect(this, &Worker::finished, &thread, &QThread::quit);
  connect(this, &Worker::finished, thread, [this]() {
    thread->quit();
    //thread->wait(); // <-- unknown - QThread::wait: Thread tried to wait on itself
    qDebug() << "Удачная остановка воркера...";
  });
  connect(this, &Worker::finished, this, &QObject::deleteLater);
  connect(thread, &QThread::finished, thread, &QObject::deleteLater);

  thread->start();
}

Worker::~Worker() {
  qDebug() << "Удачное завершение воркера...";
}

void Worker::doWork() {
  callback();
}