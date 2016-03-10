#include "MainWindow.h"
// We'll need some regular expression magic in this code:
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // When using Designer, you should always call setupUi(this)
    // in your constructor. This creates and lays out all the widgets
    // on the MainWindow that you setup in Designer.
    setupUi(this);

    // Make sure that we are showing the login page when we startup:
    stackedWidget->setCurrentWidget(loginPage);

    // Instantiate our socket (but don't actually connect to anything
    // yet until the user clicks the loginButton:
    socket = new QTcpSocket(this);

    game_start=false;
    me_x=5;
    me_y=4;
    enemy_x=5;
    enemy_y=0;
    //btn[me_y][me_x]->setIcon(QIcon());
    // This is how we tell Qt to call our readyRead() and connected()
    // functions when the socket has text ready to be read, and is done
    // connecting to the server (respectively):
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}

// This gets called when the loginButton gets clicked:
// We didn't have to use connect() to set this up because
// Qt recognizes the name of this function and knows to set
// up the signal/slot connection for us.
void MainWindow::on_loginButton_clicked()
{
    // Start connecting to the chat server (on port 4200).
    // This returns immediately and then works on connecting
    // to the server in the background. When it's done, we'll
    // get a connected() function call (below). If it fails,
    // we won't get any error message because we didn't connect()
    // to the error() signal from this socket.
    socket->connectToHost(serverLineEdit->text(), 4200);
}

// This gets called when the user clicks the sayButton (next to where
// they type text to send to the chat room):
void MainWindow::on_sayButton_clicked()
{
    // What did they want to say (minus white space around the string):
    QString message = sayLineEdit->text().trimmed();

    // Only send the text to the chat server if it's not empty:
    if(!message.isEmpty())
    {
        socket->write(QString(message + "\n").toUtf8());
    }

    // Clear out the input box so they can type something else:
    sayLineEdit->clear();

    // Put the focus back into the input box so they can type again:
    sayLineEdit->setFocus();
}

// This function gets called whenever the chat server has sent us some text:
void MainWindow::readyRead()
{
    QPushButton *buttonlist[5][11]={{pushButton,pushButton_2,pushButton_3,pushButton_4,pushButton_5,pushButton_6,pushButton_7,pushButton_8,pushButton_9,pushButton_10,pushButton_11},
                                    {pushButton_12,pushButton_13,pushButton_14,pushButton_15,pushButton_16,pushButton_17,pushButton_18,pushButton_19,pushButton_20,pushButton_21,pushButton_22},
                                    {pushButton_23,pushButton_24,pushButton_25,pushButton_26,pushButton_27,pushButton_28,pushButton_29,pushButton_30,pushButton_31,pushButton_32,pushButton_33},
                                    {pushButton_34,pushButton_35,pushButton_36,pushButton_37,pushButton_38,pushButton_39,pushButton_40,pushButton_41,pushButton_42,pushButton_43,pushButton_44},
                                    {pushButton_45,pushButton_46,pushButton_47,pushButton_48,pushButton_49,pushButton_50,pushButton_51,pushButton_52,pushButton_53,pushButton_54,pushButton_55}};
    // We'll loop over every (complete) line of text that the server has sent us:
    while(socket->canReadLine())
    {
        // Here's the line the of text the server sent us (we use UTF-8 so
        // that non-English speakers can chat in their native language)
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        // These two regular expressions describe the kinds of messages
        // the server can send us:
        //  Normal messges look like this: "username:The message"
        QRegExp messageRegex("^([^:]+):(.*)$");

        // Any message that starts with "/users:" is the server sending us a
        // list of users so we can show that list in our GUI:
        QRegExp usersRegex("^/users:(.*)$");

        // Is this a users message:
        if(usersRegex.indexIn(line) != -1)
        {
            // If so, udpate our users list on the right:
            QStringList users = usersRegex.cap(1).split(",");
            userListWidget->clear();
            foreach(QString user, users)
                new QListWidgetItem(QPixmap(":/user.png"), user, userListWidget);
        }
        // Is this a normal chat message:
        else if(messageRegex.indexIn(line) != -1)
        {
            // If so, append this message to our chat box:
            QString user = messageRegex.cap(1);
            QString message = messageRegex.cap(2);

            roomTextEdit->append("<b>" + user + "</b>: " + message);
            if(QString::compare(message,QString("START"),Qt::CaseInsensitive)==0){
                pushButton_56->setEnabled(true);
                pushButton_57->setEnabled(true);
                pushButton_58->setEnabled(true);
                pushButton_59->setEnabled(true);
            }
            else if(user==name){
                if(QString::compare(message,QString("MV_LEFT"),Qt::CaseInsensitive)==0){
                    buttonlist[me_y][me_x]->setIcon(QIcon());
                    me_x--;
                    buttonlist[me_y][me_x]->setIcon(QIcon(":/Icon/128_2.png"));
                    buttonlist[me_y][me_x]->setIconSize(QSize(50,50));
                }
                else if(QString::compare(message,QString("MV_RIGHT"),Qt::CaseInsensitive)==0){
                    buttonlist[me_y][me_x]->setIcon(QIcon());
                    me_x++;
                    buttonlist[me_y][me_x]->setIcon(QIcon(":/Icon/128_2.png"));
                    buttonlist[me_y][me_x]->setIconSize(QSize(50,50));
                }
                else if(QString::compare(message,QString("MV_UP"),Qt::CaseInsensitive)==0){
                    buttonlist[me_y][me_x]->setIcon(QIcon());
                    me_y--;
                    buttonlist[me_y][me_x]->setIcon(QIcon(":/Icon/128_2.png"));
                    buttonlist[me_y][me_x]->setIconSize(QSize(50,50));
                }
                else if(QString::compare(message,QString("MV_DOWN"),Qt::CaseInsensitive)==0){
                    buttonlist[me_y][me_x]->setIcon(QIcon());
                    me_y++;
                    buttonlist[me_y][me_x]->setIcon(QIcon(":/Icon/128_2.png"));
                    buttonlist[me_y][me_x]->setIconSize(QSize(50,50));
                }
            }
            else{
                if(QString::compare(message,QString("MV_LEFT"),Qt::CaseInsensitive)==0){
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon());
                    enemy_x--;
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon(":/Icon/128_38.png"));
                    buttonlist[enemy_y][enemy_x]->setIconSize(QSize(50,50));
                }
                else if(QString::compare(message,QString("MV_RIGHT"),Qt::CaseInsensitive)==0){
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon());
                    enemy_x++;
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon(":/Icon/128_38.png"));
                    buttonlist[enemy_y][enemy_x]->setIconSize(QSize(50,50));
                }
                else if(QString::compare(message,QString("MV_UP"),Qt::CaseInsensitive)==0){
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon());
                    enemy_y--;
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon(":/Icon/128_38.png"));
                    buttonlist[enemy_y][enemy_x]->setIconSize(QSize(50,50));
                }
                else if(QString::compare(message,QString("MV_DOWN"),Qt::CaseInsensitive)==0){
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon());
                    enemy_y++;
                    buttonlist[enemy_y][enemy_x]->setIcon(QIcon(":/Icon/128_38.png"));
                    buttonlist[enemy_y][enemy_x]->setIconSize(QSize(50,50));
                }
            }

        }
    }
}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    // Flip over to the chat page:
    stackedWidget->setCurrentWidget(chatPage);

    // And send our username to the chat server.
    socket->write(QString("/me:" + userLineEdit->text() + "\n").toUtf8());
    name=userLineEdit->text();
}



void MainWindow::on_pushButton_56_clicked()
{
    socket->write(QString("UP\n").toUtf8());
}

void MainWindow::on_pushButton_57_clicked()
{
    socket->write(QString("LEFT\n").toUtf8());
}

void MainWindow::on_pushButton_58_clicked()
{
    socket->write(QString("DOWN\n").toUtf8());
}

void MainWindow::on_pushButton_59_clicked()
{
    socket->write(QString("RIGHT\n").toUtf8());
}
