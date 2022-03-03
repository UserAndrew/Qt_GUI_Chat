#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>

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

public slots:
    void incomingConnection(qintptr socketDescriptor) override;
    void slotReadyRead();
};

#endif // SERVER_H
