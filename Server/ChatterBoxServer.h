#ifndef __ChatterBoxServer_H__
#define __ChatterBoxServer_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

class ChatterBoxServer : public QTcpServer
{
    Q_OBJECT

    public:
        ChatterBoxServer(QObject *parent=0);

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();

    protected:
        void incomingConnection(int socketfd);

    private:
        int grid[5][11];
        int client_count;
        int user1_x,user1_y,user2_x,user2_y;
        QString user1;
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
};

#endif
