#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getLogin() const {
    return login;
}
QString Dialog::getPassword() const {
    return password;
}

void Dialog::on_buttonBox_accepted()
{
    login = ui->lineEdit->text();
    password = ui->lineEdit_2->text();
}

