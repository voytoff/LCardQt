#ifndef LCARD_GLOBAL_H
#define LCARD_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QStringDecoder>
#include <QString>
#include <QMap>

#if defined(LCARD_LIBRARY)
#define LCARD_EXPORT Q_DECL_EXPORT
#else
#define LCARD_EXPORT Q_DECL_IMPORT
#endif

extern LCARD_EXPORT QStringDecoder decoder;
using EnumDesc = QMap<int, QString>;

#endif // LCARD_GLOBAL_H
