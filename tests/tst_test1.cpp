#include <QTest>
#include "crate.h"

class test1 : public QObject
{
  Q_OBJECT

public:
  test1();
  ~test1() override;

private slots:
  void initTestCase();
  void init();
  void cleanupTestCase();
  void cleanup();
  void test_case1();
};

test1::test1() {}

test1::~test1() = default;

void test1::initTestCase() {
  // code to be executed before the first test function
}

void test1::init() {
  // code to be executed before each test function
}

void test1::cleanupTestCase() {
  // code to be executed after the last test function
}

void test1::cleanup() {
  // code to be executed after each test function
}

void test1::test_case1() {
  QList<LCCrateInfo> array;
  int res = Crate::addresses(array);
  if (res == LTR_OK) {
    foreach (auto item, array) {
      qDebug() << item.flags << item.strAddress() << item.serialNumber << item.status;
    }
    Crate crate;
    crate.open(array.at(0).strAddress());
    crate.modules();
  }
}

QTEST_APPLESS_MAIN(test1)

#include "tst_test1.moc"
