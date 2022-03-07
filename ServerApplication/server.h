#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QMap>

struct FlagsFromClient
{
    const QString create = "create";
    const QString login = "login";
    const QString message = "message";
}client_flags;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    QTcpSocket* socket;

private:
    QVector<QTcpSocket*> Sockets;
    QByteArray Data;
    void sendToClient(QString str);
    qint16 nextBlockSize;
    QMap<QString, QString> messageForUser;
    QMap<QString, QMap<QString, QString>> user_data;
    void messageFromClientProcessing(QString str);


public slots:
    void incomingConnection(qintptr socketDescriptor) override;
    void slotReadyRead();
};

#endif // SERVER_H
