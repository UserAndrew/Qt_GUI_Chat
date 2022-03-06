#include "auth_window.h"
#include "ui_auth_window.h"

auth_window::auth_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::auth_window)
{
    ui->setupUi(this);
}

auth_window::~auth_window()
{
    delete ui;
}

void auth_window::on_loginPushButton_clicked()
{
    emit login_button_clicked();
}

void auth_window::on_registerPushButton_2_clicked()
{
    emit register_button_clicked();
}

void auth_window::on_lineEditLogin_textEdited(const QString &arg1)
{
    auth_window::m_username = arg1;
}

void auth_window::on_lineEditPassword_2_textEdited(const QString &arg1)
{
    auth_window::m_userpass = arg1;
}

QString auth_window::getLogin()
{
    return auth_window::m_username;
}

QString auth_window::getPass()
{
    return auth_window::m_userpass;
}
