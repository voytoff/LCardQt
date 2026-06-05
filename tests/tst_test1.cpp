#include <QTest>
#include "create.h"

// add necessary includes here

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

void test1::initTestCase()
{
  // code to be executed before the first test function
}

void test1::init()
{
  // code to be executed before each test function
}

void test1::cleanupTestCase()
{
  // code to be executed after the last test function
}

void test1::cleanup()
{
  // code to be executed after each test function
}

void test1::test_case1() {
  Create crete;
  //crete.getModuleList();
  crete.getIPAddresses();
}

QTEST_APPLESS_MAIN(test1)

#include "tst_test1.moc"
