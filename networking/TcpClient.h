#include <QTcpSocket>
#include <QTimer>
#include <QDebug>
#include "../globals.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient(const char host[], const int port, QObject *parent = nullptr) : QObject(parent)
    {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
        connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);

        tcpTimer = new QTimer(this);
        connect(tcpTimer, &QTimer::timeout, this, &TcpClient::onTimeout);

        socket->connectToHost(host, port);
    }
private slots:
    void onConnected()
    {
        qDebug() << "Connected to server";
        tcpTimer->start(globals::pacmanMovementTime);
    }

    void onDisconnected()
    {
        qDebug() << "Disconnected from server";
    }

    void onTimeout()
    {
        socket->write("read");
        if (socket->bytesAvailable() > 0)
            emitSignal();
    }
signals:
    void joystickEvent(Directions dir);

private:
    QTcpSocket *socket;
    QTimer *tcpTimer;
    char currentChar = 'L';
    void emitSignal()
    {
        for (char ch : socket->readAll())
            if (globals::charToDirection.find(ch) != globals::charToDirection.end() && currentChar != ch){
                currentChar = ch;
                // qDebug() << "Joystick value read: " << ch;
                emit joystickEvent(globals::charToDirection.at(ch));
            }
    }
};