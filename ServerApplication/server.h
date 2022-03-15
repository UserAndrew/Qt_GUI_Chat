#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QFile>
#include <QMap>

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
    ~Server();
    QTcpSocket* socket;

private:
    QVector<QTcpSocket*> Sockets;
    QByteArray Data;
    void sendToClient(QString str);
    qint16 nextBlockSize;
    QMap<QString, QString> message_for_user;
    QMap<QString, Client> user_data;
    void messageFromClientProcessing(QString str);


public slots:
    void incomingConnection(qintptr socketDescriptor) override;
    void slotReadyRead();
};

#endif // SERVER_H
