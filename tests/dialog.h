#ifndef DIALOG_H
#define DIALOG_H

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

  void accept() override;

private:
  Ui::Dialog *ui;
private slots:
  void start();
  void line(QString s);
};

#endif // DIALOG_H
