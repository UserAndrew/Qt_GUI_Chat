#include "reg_window.h"
#include "ui_reg_window.h"

reg_window::reg_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reg_window)
{
    ui->setupUi(this);
}

reg_window::~reg_window()
{
    delete ui;
}

void reg_window::on_lineEdit_Name_textEdited(const QString &arg1)
{

}

void reg_window::on_lineEdit_Password_2_textEdited(const QString &arg1)
{

}

void reg_window::on_lineEdit_Confirm_3_textEdited(const QString &arg1)
{

}

void reg_window::on_pushButtonRegister_clicked()
{

}
