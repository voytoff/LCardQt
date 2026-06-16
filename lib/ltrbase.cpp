#include "ltrbase.h"
#include <QEventLoop>
#include <QTimer>

LTRBase::LTRBase(const int &slot, QObject *parent)
  : QObject{parent}
  , slot(slot) {}

void LTRBase::pause(int msec) {
  QEventLoop loop;
  QTimer::singleShot(msec, &loop, &QEventLoop::quit);
  loop.exec();
}