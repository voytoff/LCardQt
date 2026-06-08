#ifndef LTR11_H
#define LTR11_H

#include "ilcmodule.h"
#include "lctypes.h"
#include "ltr11api.h"
#include <QObject>

class ltr11 : public QObject, public ILCModule {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ILCModule_iid)
  Q_INTERFACES(ILCModule)

public:
  ltr11(QObject *parent = nullptr);

  LCModuleInfo* info() override;

private:
  TLTR11 *hltr11;

public slots:
  bool open(const QString &addr, int slot, QString serial = nullptr);

signals:
};

#endif // LTR11_H
