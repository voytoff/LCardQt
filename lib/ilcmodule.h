#ifndef ILCMODULE_H
#define ILCMODULE_H

#include "lctypes.h"
#include <QtPlugin>

class ILCModule
{
public:
  virtual ~ILCModule() {}
  virtual LCModuleInfo* info() = 0;
};

// Associate the class with a unique identifier
#define ILCModule_iid "com.globallib.ILCModule"
Q_DECLARE_INTERFACE(ILCModule, ILCModule_iid)

#endif // ILCMODULE_H
