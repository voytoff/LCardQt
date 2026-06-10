#ifndef ILCMODULE_H
#define ILCMODULE_H

#include "lctypes.h"
#include <QtPlugin>

class LTRBase;
class ILCModule
{
public:
  virtual ~ILCModule() {}
  virtual LCModuleInfo* info() = 0;
  virtual bool open(const int &slot, const QString &serial = nullptr) = 0;
  virtual bool opened() = 0;
  virtual bool start(void *param) = 0;
  virtual bool stop() = 0;
  virtual INT error() const = 0;
  virtual LTRBase* base() const = 0;
};

// Associate the class with a unique identifier
#define ILCModule_iid "com.globallib.ILCModule"
Q_DECLARE_INTERFACE(ILCModule, ILCModule_iid)

#endif // ILCMODULE_H
