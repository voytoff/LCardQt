#ifndef LCARD_H
#define LCARD_H

#include "LCard_global.h"

class LCARD_EXPORT LCard
{
public:
  LCard();
  static QString getErrorString(int res);
};

#endif // LCARD_H
