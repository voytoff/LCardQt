#ifndef LCENUMS_H
#define LCENUMS_H

#include "LCard_global.h"
#include <QObject>
#include <QMetaEnum>

class LCARD_EXPORT LCEnums : public QObject {
  Q_OBJECT
public:
  explicit LCEnums(QObject *parent = nullptr);

  /**
   * Возвращает карту ключ-название для перечисления,
   * с использованием вспомогательного массива атрибутов
   */
  template <typename T>
  static QMap<int, QString> create(const EnumDesc &map) {
    QMap<int, QString> enumMap;
    QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
      QString name = map.value(metaEnum.keyToValue(metaEnum.key(i)));
      enumMap.insert(metaEnum.value(i), name);
    }
    return enumMap;
  }

  /** Возвращает строковое значение перечисления */
  template <typename T>
  static QString string(T key) {
    return QMetaEnum::fromType<T>().valueToKey(key);
  }
  /** Возвращает строковое значение перечисления из карты имен */
  template <typename T>
  static QString string(T key, const EnumDesc &map) {
    return map.value(key);
  }

  /** Типы крейтов */
  enum LTR_CrateType : int {
    LTR_CRATE_TYPE_UNKNOWN        = 0, /** Неизвестный тип крейта */
    LTR_CRATE_TYPE_LTR010         = 10, /** Крейт LTR-U-8 или LTR-U-16 */
    LTR_CRATE_TYPE_LTR021         = 21, /** Крейт LTR-U-1 */
    LTR_CRATE_TYPE_LTR030         = 30, /** Крейт LTR-EU-8 или LTR-EU-16 ** 16/8-местный крейт с интерфейсами USB 2.0, Ethernet и источником питания. */
    LTR_CRATE_TYPE_LTR031         = 31, /** Крейт LTR-EU-2 2-местный портативный крейт с интерфейсами USB 2.0, Ethernet и источником питания. */
    LTR_CRATE_TYPE_LTR032         = 32, /** Крейт LTR-E-7 или LTR-E-15 */
    LTR_CRATE_TYPE_LTR_CU_1       = 40, /** Крейт LTR-CU-1 */
    LTR_CRATE_TYPE_LTR_CEU_1      = 41, /** Крейт LTR-CEU-1 */
    LTR_CRATE_TYPE_BOOTLOADER     = 99  /** Крейт в режиме загрузчика (если в этом режиме нельзя узнать тип) */
  };
  Q_ENUM(LTR_CrateType)
  static const EnumDesc LTR_CrateTypeMap;

  /** Интерфейс подключения крейта */
  enum LTR_CrateIface : int {
    LTR_CRATE_IFACE_UNKNOWN   = 0, /** Неизвествный код интерфейса крейта.
                                    При передаче в функции данное значение
                                    может указывать, что интерфейс подклучения
                                    крейта не имеет значения */
    LTR_CRATE_IFACE_USB       = 1, /** Крейт подключен по интерфейсу USB */
    LTR_CRATE_IFACE_TCPIP     = 2  /** Крейт подключен по Ethernet (TCP/IP) */
  };
  Q_ENUM(LTR_CrateIface)
  static const EnumDesc LTR_CrateIfaceMap;

  /** Режим работы крейта */
  enum LTR_CrateMode {
    LTR_CRATE_MODE_BOOTLOADER = 1, /** Крейт находится в состоянии загрузчика */
    LTR_CRATE_MODE_WORK       = 2, /** Крейт находится в рабочем состоянии */
    LTR_CRATE_MODE_CONTROL    = 3  /** Крейт находится в состоянии, когда принимает
                                     только управляющие запросы (например, если
                                     крейт подключен не по тому интерфейсу, на который
                                     нестроен) */
  };
  Q_ENUM(LTR_CrateMode)
  static const EnumDesc LTR_CrateModeMap;

};

#endif // LCENUMS_H
