#include "dialog.h"
#include "crate.h"
#include "ui_dialog.h"
#include <QString>
#include <QPushButton>

Dialog::~Dialog() {
  delete ui;
}

void Dialog::accept() {
  return;
}

Dialog::Dialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::Dialog)
{
  ui->setupUi(this);
  connect(ui->buttonBox, &QDialogButtonBox::clicked, this, [this](QAbstractButton *button) {
    if (button == ui->buttonBox->button(QDialogButtonBox::Retry))
      start();
  });
}

void Dialog::line(QString s) {
  QMetaObject::invokeMethod(this, [this, s]() {
    ui->log->append(s);
  }, Qt::QueuedConnection);
}

void Dialog::start() {
  Crate crate;
  if (crate.open()) {
    foreach (auto m, crate.hardware()) {
      auto i = m->info();
      line(QString("%1 : %2 : %3 : %4 : %5 : %6").arg(i->slot).arg(i->type).arg(i->name, i->serial).arg(i->version).arg(i->date.toString()));
    }
  }
  else {
    line(crate.lastError());
    return;
  }

  LCChannels11 ch11;
  /* диапазон - 10В, режим - 16-канальный, физический канал - 1 */
  ch11.insert(0, LCChannel11 {0, LTR11_CHMODE_DIFF, LTR11_CHRANGE_10000MV});
  /* диапазон - 2.5В, режим - измерение собственного нуля, физический канал - 2 */
  ch11.insert(1, LCChannel11 {1, LTR11_CHMODE_ZERO, LTR11_CHRANGE_2500MV});
  /* диапазон - 0.6В, режим - 32-канальный (каналы 1..16), физический канал - 3 */
  ch11.insert(2, LCChannel11 {2, LTR11_CHMODE_COMM, LTR11_CHRANGE_625MV});
  /* диапазон - 0.15В, режим - 32-канальный (каналы 17..32), физический канал - 25 */
  ch11.insert(3, LCChannel11 {24, LTR11_CHMODE_COMM, LTR11_CHRANGE_156MV});
  LCParameters *params = new LCParameters();
  params->insert(2, new LCParam11 {
    // Сбор данных начнётся по вызову LTR11_Start() (TODO: по изменению внешнего сигнала)
    LTR11_STARTADCMODE_INT,
    // АЦ-преобразования будут происходить с заданной частотой
    LTR11_INPMODE_INT,
    // Режим работы модуля - сбор данных
    LTR11_ADCMODE_ACQ,
    ch11
  });

  int n = 0;
  connect(&crate, &Crate::dataReady, this, [&n, this](LTRBase *module, const int &count, double *data) {
    QString s;
    s.append(QString("Блок: %1").arg(n++));
    for (int i = 0; i < count; i++) {
      s.append(QString::number(data[i]));
      if (i == (count-1)) {
        s.append("\n");
      } else {
        s.append(", ");
      }
    }
    line(s);
  });

  if (!crate.start(params)) {
    line(crate.lastError());
    return;
  }
  //if (!crate.stop() || !crate.close()) {
  //  line(crate.lastError());
  //  return;
  //}
}
