#ifndef DIALOG_H
#define DIALOG_H

#include "crate.h"
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();

  void accept() override {};
  void reject() override;

private:
  Ui::Dialog *ui;
  Crate *crate = nullptr;
  bool closed = false;

private slots:
  bool start();
  void line(QString s);
};

#endif // DIALOG_H
