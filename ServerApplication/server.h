#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QFile>
#include <QMap>
#include <QCryptographicHash>

struct Client
{
    QString password;
    QString name;
};

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server(){};
    QTcpSocket* socket;

private:
    qintptr this_socketDescritor;
    QVector<QTcpSocket*> Sockets;
    QByteArray Data;
    void sendToClient(QString);
    qint16 nextBlockSize;
    QMap<qintptr, QString> socket_descriptor_and_name;
    QMap<QString, QString> message_for_user;
    QMap<QString, Client> user_data;
    void messageFromClientProcessingAndSending(QString);
    void writeUsersDataToFile(QStringList);
    void preparingDataToSend(QString);


public slots:
    void incomingConnection(qintptr) override;
    void slotReadyRead();
};

#endif // SERVER_H
