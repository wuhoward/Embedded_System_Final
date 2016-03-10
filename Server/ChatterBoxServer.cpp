#include "ChatterBoxServer.h"

#include <QTcpSocket>
#include <QRegExp>

ChatterBoxServer::ChatterBoxServer(QObject *parent) : QTcpServer(parent)
{
    client_count=0;
    user1_x=5;
    user1_y=4;
    user2_x=5;
    user2_y=0;
    for(int i=0;i<5;i++){
        for(int j=0;j<11;j++){
            grid[i][j]=0;
        }
    }
    grid[user1_y][user1_x]=1;
    grid[user2_y][user2_x]=2;
    grid[1][4]=3;
    grid[1][5]=3;
    grid[1][6]=3;
    grid[3][4]=3;
    grid[3][5]=3;
    grid[3][6]=3;
}

void ChatterBoxServer::incomingConnection(int socketfd)
{
    if(client_count<2){
        QTcpSocket *client = new QTcpSocket(this);
        client->setSocketDescriptor(socketfd);
        clients.insert(client);

        qDebug() << "New client from:" << client->peerAddress().toString();

        connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
        client_count++;
    }
}

void ChatterBoxServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            users[client] = user;
            if(client_count==1)user1=user;
            foreach(QTcpSocket *client, clients){
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
                if(client_count==2)client->write(QString("Server:START\n").toUtf8());
            }

            sendUserList();
        }
        else if(users.contains(client))
        {
            QString message = line;
            QString user = users[client];
            qDebug() << "User:" << user;
            qDebug() << "Message:" << message;
            QStringList cmd = message.split(" \n");
            if(QString::compare(cmd[0],QString("LEFT"),Qt::CaseInsensitive)==0
                    &&((user==user1&&user1_x!=0&&grid[user1_y][user1_x-1]==0)
                    ||(user!=user1&&user2_x-1!=0&&grid[user2_y][user2_x-1]==0))){
                if(user==user1){
                    grid[user1_y][user1_x]=0;
                    grid[user1_y][user1_x-1]=1;
                    user1_x--;
                }
                else{
                    grid[user2_y][user2_x]=0;
                    grid[user2_y][user2_x-1]=2;
                    user2_x--;
                }
                foreach(QTcpSocket *otherClient, clients)
                    otherClient->write(QString(user + ":" + "MV_LEFT" + "\n").toUtf8());
            }
            else if(QString::compare(cmd[0],QString("RIGHT"),Qt::CaseInsensitive)==0
                    &&((user==user1&&user1_x!=10&&grid[user1_y][user1_x+1]==0)
                    ||(user!=user1&&user2_x+1!=10&&grid[user2_y][user2_x+1]==0))){
                if(user==user1){
                    grid[user1_y][user1_x]=0;
                    grid[user1_y][user1_x+1]=1;
                    user1_x++;
                }
                else{
                    grid[user2_y][user2_x]=0;
                    grid[user2_y][user2_x+1]=2;
                    user2_x++;
                }
                foreach(QTcpSocket *otherClient, clients)
                    otherClient->write(QString(user + ":" + "MV_RIGHT" + "\n").toUtf8());
            }
            else if(QString::compare(cmd[0],QString("UP"),Qt::CaseInsensitive)==0
                    &&((user==user1&&user1_y!=0&&grid[user1_y-1][user1_x]==0)
                    ||(user!=user1&&user2_y!=4&&grid[user2_y+1][user2_x]==0))){
                if(user==user1){
                    grid[user1_y][user1_x]=0;
                    grid[user1_y-1][user1_x]=1;
                    user1_y--;
                }
                else{
                    grid[user2_y][user2_x]=0;
                    grid[user2_y+1][user2_x]=2;
                    user2_y++;
                }
                foreach(QTcpSocket *otherClient, clients)
                    if(users[otherClient]==user)otherClient->write(QString(user + ":" + "MV_UP" + "\n").toUtf8());
                    else otherClient->write(QString(user + ":" + "MV_DOWN" + "\n").toUtf8());
            }
            else if(QString::compare(cmd[0],QString("DOWN"),Qt::CaseInsensitive)==0
                    &&((user==user1&&user1_y!=4&&grid[user1_y+1][user1_x]==0)
                    ||(user!=user1&&user2_y!=0&&grid[user2_y-1][user2_x]==0))){
                if(user==user1){
                    grid[user1_y][user1_x]=0;
                    grid[user1_y+1][user1_x]=1;
                    user1_y++;
                }
                else{
                    grid[user2_y][user2_x]=0;
                    grid[user2_y-1][user2_x]=2;
                    user2_y--;
                }
                foreach(QTcpSocket *otherClient, clients)
                    if(users[otherClient]==user)otherClient->write(QString(user + ":" + "MV_DOWN" + "\n").toUtf8());
                    else otherClient->write(QString(user + ":" + "MV_UP" + "\n").toUtf8());
            }

            /*foreach(QTcpSocket *otherClient, clients)
                otherClient->write(QString(user + ":" + message + "\n").toUtf8());*/
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void ChatterBoxServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    client_count--;
    clients.remove(client);

    QString user = users[client];
    users.remove(client);

    sendUserList();
    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void ChatterBoxServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users.values())
        userList << user;

    foreach(QTcpSocket *client, clients)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}
