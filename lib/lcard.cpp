#include "lcard.h"
#include "ltrapi.h"
#include <QStringConverter>

QStringDecoder decoder = QStringDecoder(QStringConverter::System);

LCard::LCard() {}

QString LCard::getErrorString(int res) {
  return QString(decoder.decode(LTR_GetErrorString(res)));
}


