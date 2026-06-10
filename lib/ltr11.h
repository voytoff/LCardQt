#ifndef LTR11_H
#define LTR11_H

#include "crate.h"
#include "ilcmodule.h"
#include "lctypes.h"
#include "ltr11api.h"
#include "ltrbase.h"
#include <QObject>

class ltr11 :/* public QObject,*/ public LTRBase, public ILCModule {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ILCModule_iid)
  Q_INTERFACES(ILCModule)

public:
  explicit ltr11(QObject *parent = nullptr);

  LCModuleInfo* info() override;
  inline Crate *crate() {return (Crate*)parent();}
  LTRBase* base() const override  {return (LTRBase*)this;}

private:
  INT result;
  /* признак необходимости завершить сбор данных */
  int f_out;

private:
  TLTR11 *ltr;
  int slot;
  void dataThreadFunction(const int &dataBuferLength, double *data, bool &out);

public slots:
  bool open(const int &slot, const QString &serial = nullptr) override;
  bool opened() override;
  bool start(void *param) override;
  bool stop() override;
  bool close();
  INT error() const override {return result;}
  QString lastError() const {return LCard::getErrorString(result);}

signals:

};

#endif // LTR11_H
