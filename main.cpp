#include "widgets/MainView.h"
#include "widgets/LostView.h"
#include "widgets/WelcomeView.h"
#include "networking/TcpClient.h"
#include <QApplication>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    QApplication a(argc, argv);
    TcpClient client("192.168.1.28", 8080);

    WelcomeView welcome;
    welcome.show();

    MainView window;
    window.hide();

    LostView lost;
    lost.hide();

    a.connect(&client, &TcpClient::joystickEvent, window.getBoard(), &Board::setPlayerBufforDirection);

    a.connect(welcome.startButton, &QPushButton::clicked, [&](){
        window.show();
        window.getBoard()->actionDelayTimer->start(globals::actionDelayTime);
        welcome.hide();
    });

    a.connect(&window, &MainView::gameLost, &a, [&](int points){
        lost.lostLabel->update(points);
        lost.show();
        window.hide();
    });
    
    return a.exec();
}
