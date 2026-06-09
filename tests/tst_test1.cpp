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
  if (res != LTR_OK) return;
  foreach (auto item, array) {
    qDebug() << item.flags << item.strAddress() << item.serialNumber << item.status;
  }

  QString addr = nullptr;
  if (array.length() > 0) addr = array.at(0).strAddress();

  Crate crate(addr);
  if (crate.open()) {
    foreach (auto m, crate.hardware()) {
      auto i = m->info();
      qDebug() << i->slot << i->type << i->name << i->serial << i->version << i->date;
    }
  }
  else {
    qDebug() << crate.lastError();
    return;
  }
  LCChannels11 ch11;
  ch11.insert(2, LCChannel11 {1, LTR11_CHMODE_ZERO, LTR11_CHRANGE_2500MV});
  LCParameters *params = new LCParameters();
  params->insert(2, new LCParam11 {
    LTR11_STARTADCMODE_INT,
    LTR11_INPMODE_INT,
    LTR11_ADCMODE_ACQ,
    ch11
  });
  if (!crate.start(params)) {
    qDebug() << crate.lastError();
    return;
  }
  if (!crate.stop() || !crate.close()) {
    qDebug() << crate.lastError();
    return;
  }
}

QTEST_APPLESS_MAIN(test1)

#include "tst_test1.moc"
