#include "lcenums.h"

LCEnums::LCEnums(QObject *parent) : QObject{parent} { }

const EnumDesc LCEnums::LTR_CrateTypeMap = {
  {LCEnums::LTR_CRATE_TYPE_UNKNOWN, "Неизвестный тип крейта"},
  {LCEnums::LTR_CRATE_TYPE_LTR010, "LTR-U-8/16"},
  {LCEnums::LTR_CRATE_TYPE_LTR021, "LTR-U-1"},
  {LCEnums::LTR_CRATE_TYPE_LTR030, "LTR-EU-8/16"},
  {LCEnums::LTR_CRATE_TYPE_LTR031, "LTR-EU-2"},
  {LCEnums::LTR_CRATE_TYPE_LTR032, "LTR-E-7/15"},
  {LCEnums::LTR_CRATE_TYPE_LTR_CU_1,"LTR-CU-1"},
  {LCEnums::LTR_CRATE_TYPE_LTR_CEU_1, "LTR-CEU-1"},
  {LCEnums::LTR_CRATE_TYPE_BOOTLOADER, "Крейт в режиме загрузчика?"}
};

const EnumDesc LCEnums::LTR_CrateIfaceMap = {
  {LCEnums::LTR_CrateIface::LTR_CRATE_IFACE_UNKNOWN, "Неизвествный код интерфейса крейта"},
  {LCEnums::LTR_CrateIface::LTR_CRATE_IFACE_USB, "Крейт подключен по интерфейсу USB"},
  {LCEnums::LTR_CrateIface::LTR_CRATE_IFACE_TCPIP, "Крейт подключен по Ethernet (TCP/IP)"},
};
