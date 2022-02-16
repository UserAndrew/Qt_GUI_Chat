#include "server.h"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2000))
    {
        qDebug() << "Start server success.";
    }
    else
    {
        qDebug() << "Start server error!";
    }
}

void Server::incomigConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);

    qDebug() << "Client connected! Its descriptor " << socketDescriptor;
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Read...";
        QString str;
        in >> str;
        qDebug() << str;
        sendToClient(str);
    }
    else
    {
        qDebug() << "DataStream error.";
    }
}

void Server::sendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    //socket->write(Data);
    for(int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}
