#include "server.h"

QString getSeparator()
{
    return "|";
}

struct AnswerToClient
{
    const std::pair<QString, QString> login_exist{"error","This login already exist"};
    const std::pair<QString, QString> incorrect_login{"error","Incorrect login"};
    const std::pair<QString, QString> incorrect_pass{"error","Incorrect password"};
    const QString reg_ok{"registration ok"};
    const QString auth_ok{"authentification ok"};
    const QString message_to_clients{"message"};
} answer_to_client;

struct FlagsFromClient
{
    const QString create{"create"};
    const QString login{"login"};
    const QString message{"message"};
} client_flags;

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
    QFile file("users_data.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("Cannot open file for reading");
    }
    else
    {
        QTextStream in(&file);
        Client datas;
        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList list = line.split("\t");
            datas.name = list[1];
            datas.password = list[2];
            user_data[list[0]] = datas;
        }
    }
    file.close();
    /*file.open(QFile::WriteOnly|QFile::Truncate);
    file.close();
    QMap<QString,Client>::iterator it = user_data.begin();
    for(; it != user_data.end(); it++)
    {
        qDebug()<<it.key()<<"\t"<<it.value().name<<"\t"<<it.value().password<<endl;
    }*/
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
            messageFromClientProcessing(str);
            //sendToClient(str);
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

void Server::writeUsersDataToFile(QStringList list)
{
    QTextStream out(stdout);
    QString filename = "users_data.txt";
    QFile file(filename);
    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<list[1]<<"\t"<<list[2]<<"\t"<<list[3]<<endl;
    }
    else
    {
        qWarning("Could not open file");
    }

    file.close();
}

void Server::messageFromClientProcessing(QString str)
{
    Client data_client;
    QString my_string;
    QStringList list = str.split("|");
    if(list[0] == client_flags.create)
    {
        if(!user_data.count(list[1]))
        {
            data_client.name = list[2];
            data_client.password = list[3];
            user_data[list[1]] = data_client;
            writeUsersDataToFile(list);
            my_string = answer_to_client.reg_ok+getSeparator();
        }
        else
        {
            my_string = answer_to_client.incorrect_login.first+getSeparator()+
                    answer_to_client.incorrect_login.second;
        }
    }
    else if(list[0] == client_flags.login)
    {
        if(user_data.count(list[1]))
        {
            if(user_data[list[1]].password == list[2])
            {
                qintptr abc = socketDescriptor();
                my_string = answer_to_client.auth_ok+getSeparator();//+user_data[list[1]].name;
                socket_descriptor_and_name.insert(abc, user_data[list[1]].name);
                qDebug()<<socket_descriptor_and_name.values()<<'\t'<<
                          socket_descriptor_and_name.keys();
            }
            else
            {
                my_string = answer_to_client.incorrect_pass.first+getSeparator()+
                        answer_to_client.incorrect_pass.second;
            }
        }
        else
        {
            my_string = answer_to_client.incorrect_login.first+getSeparator()+
                    answer_to_client.incorrect_login.second;
        }
    }
    else if(list[0] == client_flags.message)
    {
        QString name;
        QMap<qintptr, QString>::iterator it = socket_descriptor_and_name.begin();
        for(; it != socket_descriptor_and_name.end(); it++)
        {
            if(it.key() == socketDescriptor())
            {
                name = it.value();
            }
        }
        my_string = answer_to_client.message_to_clients+getSeparator()+
                name+getSeparator()+list[1];
    }

    sendToClient(my_string);
}

