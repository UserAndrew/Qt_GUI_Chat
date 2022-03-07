#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>

#include "dialog.h"

//#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Client
{
    std::string login;
    std::string password;
    std::string name;
};

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

    Dialog* dg;

    void sendToServer(QString str);
    qint16 nextBlockSize;

    QString m_username;
    QString m_userpass;

    bool m_loginSuccesfull;

public slots:
    void slotReadyRead();

};
#endif // MAINWINDOW_H
