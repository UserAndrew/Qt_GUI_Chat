#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QtDebug>
#include <QMessageBox>

QString getSeparator()
{
    return "|";
}

struct FlagsForServer
{
    const QString create = "create";
    const QString login = "login";
    const QString message = "message";
} serverFlags;

struct SignalsFromServer
{
    const QString error{"error"};
    const QString message{"message"};
    const QString regOK{"registration ok"};
    const QString authOK{"authentication ok"};
} signals_from_server;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 2000);
    m_loginSuccesfull = false;
    connect(&ui_Auth, SIGNAL(login_button_clicked()),
            this, SLOT(authorizeUser()));
    connect(&ui_Auth, SIGNAL(destroyed()),
            this, SLOT(show()));
    connect(&ui_Auth, SIGNAL(register_button_clicked()),
            this, SLOT(registerWindowShow()));
    connect(&ui_Reg, SIGNAL(register_button_clicked2()),
            this, SLOT(registerUser()));
    connect(&ui_Reg, SIGNAL(destroyed()),
            &ui_Auth, SLOT(show()));

    /*ui->setupUi(this);
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 2000);*/
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerWindowShow()
{
    ui_Auth.hide();
    ui_Reg.show();
}

void MainWindow::authorizeUser()
{
    m_username = ui_Auth.getLogin();
    m_userpass = ui_Auth.getPass();
    QString auth_string = serverFlags.login+getSeparator()+
            m_username+getSeparator()+m_userpass;
    sendToServer(auth_string);
}

void MainWindow::registerUser()
{
    m_username = ui_Reg.getName();
    m_userlogin = ui_Reg.getLogin();
    m_userpass = ui_Reg.getPass();
    QString reg_string = serverFlags.create+getSeparator()+
            m_userlogin+getSeparator()+m_username+getSeparator()+m_userpass;
    sendToServer(reg_string);
}

void MainWindow::display()
{
    ui_Auth.show();
}

void MainWindow::sendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << qint16(0) << str;
    out.device()->seek(0);
    out << qint16(Data.size() - sizeof(qint16));
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::messageFromServerProcessing(QString str)
{
    QStringList list = str.split("|");
    if(list[0] == signals_from_server.error)
    {
        QMessageBox::warning(this, "Warning!", list[1]);
    }
    else if(list[0] == signals_from_server.message)
    {

        //ui->textBrowser->append(list[1]);
    }
    else if(list[0] == signals_from_server.regOK)
    {
        ui_Reg.close();
        ui_Auth.show();
    }
    else if(list[0] == signals_from_server.authOK)
    {
        ui_Auth.close();
    }
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        /*QString str;
        in >> str;
        ui->textBrowser->append(str);*/
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }
            QString str;
            in >> str;
            nextBlockSize = 0;
            messageFromServerProcessing(str);
            //ui->textBrowser->append(str);
        }
    }
    else
    {
        ui->textBrowser->append("Read error!");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    sendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    sendToServer(ui->lineEdit->text());
}
