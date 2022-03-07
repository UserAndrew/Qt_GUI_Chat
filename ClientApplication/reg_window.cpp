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
    reg_window::m_username = arg1;
}

void reg_window::on_lineEdit_Login_textEdited(const QString &arg1)
{
    reg_window::m_userlogin = arg1;
}

void reg_window::on_lineEdit_Password_2_textEdited(const QString &arg1)
{
    reg_window::m_userpass = arg1;
}

void reg_window::on_lineEdit_Confirm_3_textEdited(const QString &arg1)
{
    reg_window::m_confirmation = arg1;
}

void reg_window::on_pushButtonRegister_clicked()
{
    emit register_button_clicked2();
}

QString reg_window::getName()
{
    return m_username;
}

QString reg_window::getLogin()
{
    return m_userlogin;
}

QString reg_window::getPass()
{
    return m_userpass;
}

bool reg_window::checkPass()
{
    return (m_confirmation == m_userpass);
}
