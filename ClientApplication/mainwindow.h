#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include "auth_window.h"
#include "reg_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void display();

private slots:
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
    void authorizeUser();
    void registerWindowShow();
    void registerUser();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;

    void sendToServer(QString str);
    void messageFromServerProcessing(QString str);
    qint16 nextBlockSize;
    auth_window ui_Auth;
    reg_window ui_Reg;

    QString m_username;
    QString m_userpass;
    QString m_userlogin;

    QVector<QString> messages_history_from_server;

    bool m_loginSuccesfull;

public slots:
    void slotReadyRead();

};
#endif // MAINWINDOW_H
