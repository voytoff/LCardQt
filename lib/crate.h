#ifndef CREATE_H
#define CREATE_H

#include "lcard.h"
#include "lcenums.h"
#pragma hdrstop
#ifdef _WIN32
#include <conio.h>
#include <locale.h>
#include <winsock2.h> // Важно подключать перед ltrapi для работы сетевых типов
#else
#include <signal.h>
#include <unistd.h>
#endif
#pragma argsused
#include "ltrapi.h"

#include "LCard_global.h"
#include "lctypes.h"
#include "ltrbase.h"

#include <QObject>
#include <QStringDecoder>
#include <QList>
#include <QHash>

class LCARD_EXPORT Crate : public QObject {
  Q_OBJECT
public:
  explicit Crate(const QString &addr = nullptr, QObject *parent = nullptr);

private:
  QString address;
  TLTR *ltr;
  INT result;
  /** хеш таблицу модулей */
  LCParameters *params = nullptr;

protected:
  bool init();
  LTRBase *module(const int &slot, const int &type);

public:
  static int addresses(QList<LCCrateInfo> &array);
  /** Получение серийных номеров подключенных крейтов */
  static int crates(QList<QString> &list);
  /** Получение информации о подключенных крейтах */
  static int cratesEx(QList<CrateInfo> &list);
  /** Получение описания крейта */
  static int crateInfo(const QString &serial, CrateInfoEx &info OUT);
  /** Получение статистики по крейту */
  static int crateStat(const QString &serial, CrateStatistic &statistic OUT);

  QHash<int, LTRBase*> *_modules;

  bool open();
  bool close();
  bool start(LCParameters *params);
  bool opened();
  QString version();
  INT error() const {return result;}
  QString lastError() const {return LCard::getErrorString(result);}
  QString &ip() {return address;}
  /** возвращает информацию о модуле крейта
   *  key = номер слота
   */
  QHash<int, LTRBase*> &hardware() const {return *_modules;}

private slots:

signals:
  void dataReady(LTRBase *module, const int &count, double *data);

};

#endif // CREATE_H
