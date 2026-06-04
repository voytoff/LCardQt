#ifndef LCARD_GLOBAL_H
#define LCARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LCARD_LIBRARY)
#define LCARD_EXPORT Q_DECL_EXPORT
#else
#define LCARD_EXPORT Q_DECL_IMPORT
#endif

#endif // LCARD_GLOBAL_H
