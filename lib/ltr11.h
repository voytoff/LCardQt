#ifndef LTR11_H
#define LTR11_H

#include "crate.h"
#include "ltr11api.h"
#include "ltrbase.h"
#include <QObject>

class ltr11 : public LTRBase {
  Q_OBJECT

public:
  explicit ltr11(const int &slot, QObject *parent = nullptr);

  inline Crate *crate() {return (Crate*)parent();}
  LTRBase* base() const override  {return (LTRBase*)this;}

private:
  TLTR11 *ltr = nullptr;
  void dataThreadFunction(const int &dataBuferLength, double *data);

public slots:
  bool open(const QString &serial = nullptr) override;
  bool opened() override;
  bool start(void *param) override;
  bool close() override;
  LCModuleInfo* info() override;

signals:

};

#endif // LTR11_H
