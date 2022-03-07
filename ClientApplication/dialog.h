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

    QString getLogin() const;
    QString getPassword() const;
private slots:
    void on_buttonBox_accepted();

private:
    QString login;
    QString password;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
