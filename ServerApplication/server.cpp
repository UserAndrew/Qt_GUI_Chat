#include "server.h"

QString getYou()
{
    return "You";
}

struct Separator
{
    const QString pipeline{"|"};
    const QString two_point{":"};
} separator;

struct AnswerToClient
{
    const std::pair<QString, QString> login_exist{"error","This login already exist"};
    const std::pair<QString, QString> incorrect_login{"error","Incorrect login"};
    const std::pair<QString, QString> incorrect_pass{"error","Incorrect password"};
    const QString reg_ok{"registration ok"};
    const QString auth_ok{"authentification ok"};
    const QString message_to_clients{"message"};
    const QString messages_history{"messages history"};
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
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting... ";
    this_socketDescritor = socketDescriptor;
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
            messageFromClientProcessingAndSending(str);
            break;
        }
    }
    else
    {
        qDebug() << "DataStream error.";
    }
}

void Server::preparingDataToSend(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << qint16(0) << str;
    out.device()->seek(0);
    out << qint16(Data.size() - sizeof(qint16));
}

void Server::writeMessagesHistory(QString message)
{
    QTextStream out(stdout);
    QString filename = "messages_history.txt";
    QFile file(filename);
    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<message<<endl;
    }

    file.close();
}

void Server::readMessagesHistory()
{
    QFile file("messages_history.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("Cannot open file for reading");
    }
    else
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString name;
            QString line = in.readLine();
            QStringList list = line.split(separator.pipeline);
            qintptr this_socket_Descriptor = socket->socketDescriptor();
            if(list[0] == socket_descriptor_login_and_name[this_socket_Descriptor].login)
            {
                name = getYou()+separator.two_point;
            }
            else
            {
                name = list[1]+separator.two_point;
            }
            messages_history.insert(name, list[2]);
        }
    }

    file.close();
}

QString Server::passwordEncryption(QString passw)
{
    QString md5_string = QCryptographicHash::hash(passw.toLatin1(),
            QCryptographicHash::Md5).toHex();
    return md5_string;
}

void Server::sendToClient(QString str)
{
    preparingDataToSend(str);
    socket->write(Data);
}

void Server::writeUsersDataToFile(QStringList list)
{
    QString encrypt_pass = passwordEncryption(list[3]);
    QTextStream out(stdout);
    QString filename = "users_data.txt";
    QFile file(filename);
    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<list[1]<<"\t"<<list[2]<<"\t"<<encrypt_pass<<"\t"<<list[3]<<endl;
    }
    else
    {
        qWarning("Could not open file");
    }

    file.close();
}

void Server::messageFromClientProcessingAndSending(QString str)
{
    Client data_client;
    ClientId client_id;
    QString my_string;
    QStringList list = str.split(separator.pipeline);
    if(list[0] == client_flags.create)
    {
        if(!user_data.count(list[1]))
        {
            data_client.name = list[2];
            data_client.password = passwordEncryption(list[3]);
            user_data[list[1]] = data_client;
            writeUsersDataToFile(list);
            my_string = answer_to_client.reg_ok+separator.pipeline;
        }
        else
        {
            my_string = answer_to_client.login_exist.first+separator.pipeline+
                    answer_to_client.login_exist.second;
        }
    }
    else if(list[0] == client_flags.login)
    {
        if(user_data.count(list[1]))
        {
            QString this_pass = passwordEncryption(list[2]);
            if(user_data[list[1]].password == this_pass)
            {
                my_string = answer_to_client.auth_ok+separator.pipeline+user_data[list[1]].name;
                client_id.login = list[1];
                client_id.name = user_data.value(list[1]).name;
                socket_descriptor_login_and_name.insert(this_socketDescritor, client_id);
                qDebug()<<socket_descriptor_login_and_name.values()<<'\t'<<
                          socket_descriptor_login_and_name.keys();
                readMessagesHistory();
                QMap<QString,QString>::iterator it = messages_history.begin();
                for(; it != messages_history.end(); it++)
                {
                    sendToClient(answer_to_client.messages_history+separator.pipeline+
                                 it.key()+separator.two_point+it.value());
                }
                messages_history.clear();
            }
            else
            {
                my_string = answer_to_client.incorrect_pass.first+separator.pipeline+
                        answer_to_client.incorrect_pass.second;
            }
        }
        else
        {
            my_string = answer_to_client.incorrect_login.first+separator.pipeline+
                    answer_to_client.incorrect_login.second;
        }
    }
    else if(list[0] == client_flags.message)
    {
        QString login = socket_descriptor_login_and_name[socket->socketDescriptor()].login;
        QString u_name = socket_descriptor_login_and_name[socket->socketDescriptor()].name;
        writeMessagesHistory(login+separator.pipeline+u_name+separator.pipeline+
                             list[1]+separator.pipeline);

        QString name;
        for(int i = 0; i < Sockets.size(); i++)
        {
            if(Sockets[i]->socketDescriptor() == socket->socketDescriptor())
            {
                name = getYou();
            }
            else
            {
                name = u_name;
            }
            my_string = answer_to_client.message_to_clients+separator.pipeline+
                            name+separator.pipeline+list[1];
            preparingDataToSend(my_string);
            Sockets[i]->write(Data);
        }
        return;
    }

    sendToClient(my_string);
}

