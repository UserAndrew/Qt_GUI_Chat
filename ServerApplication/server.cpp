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
    nextBlockSize = 0;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting... ";
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
        /*qDebug() << "Read...";
        QString str;
        in >> str;
        qDebug() << str;
        sendToClient(str);*/
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize = 0 ";
                if(socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break.";
                    break;
                }
                in >> nextBlockSize;
                qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not full, break.";
                break;
            }
            QString str;
            in >> str;
            qDebug() << str;
            nextBlockSize = 0;
            sendToClient(str);
            break;
        }
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
    out << qint16(0) << str;
    out.device()->seek(0);
    out << qint16(Data.size() - sizeof(qint16));
    //socket->write(Data);
    for(int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}
