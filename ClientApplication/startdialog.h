#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QWidget>

namespace Ui {
class StartDialog;
}

class StartDialog : public QWidget
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
