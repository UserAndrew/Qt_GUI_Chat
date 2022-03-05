#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include "auth_window.h"
#include "reg_window.h"

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

private slots:
    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    //StartDialog ui_start_dialog;
    void sendToServer(QString str);
    qint16 nextBlockSize;

public slots:
    void slotReadyRead();
};
#endif // MAINWINDOW_H
