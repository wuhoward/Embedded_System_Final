// We need to include a couple Qt classes that we'll use:
#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
//#include <QPushButton>

// This is the include file that Qt generates for us from the
// GUI we built in Designer  
#include "ui_MainWindow.h"

/*
 * This is the MainWindow class that we have told to inherit from
 * our Designer MainWindow (ui::MainWindow)
 */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:

        // Every QWidget needs a constructor, and they should allow
        // the user to pass a parent QWidget (or not).
        MainWindow(QWidget *parent=0);


    private slots:

        // This function gets called when a user clicks on the
        // loginButton on the front page (which you placed there
        // with Designer)
        void on_loginButton_clicked();

        // This gets called when you click the sayButton on
        // the chat page.
        void on_sayButton_clicked();

        // This is a function we'll connect to a socket's readyRead()
        // signal, which tells us there's text to be read from the chat
        // server.
        void readyRead();

        // This function gets called when the socket tells us it's connected.
        void connected();


        void on_pushButton_56_clicked();

        void on_pushButton_57_clicked();

        void on_pushButton_58_clicked();

        void on_pushButton_59_clicked();

private:
        bool game_start;
        QString name;
        int me_x,me_y,enemy_x,enemy_y;
        // This is the socket that will let us communitate with the server.
        QTcpSocket *socket;
};
