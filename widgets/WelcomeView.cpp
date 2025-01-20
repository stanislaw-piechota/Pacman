#include "WelcomeView.h"
#include "globals.h"
#include <QPixmap>

WelcomeView::WelcomeView(){
    titleLabel = new UpdatableLabel("PAC-MAN", createFont(48, globals::pacFontPath), "yellow", this);
    setStyleSheet("background-color: black; font-weight: bold;");
    setFixedSize(globals::boardWidth, globals::boardHeight);
    titleLabel->setFixedSize(globals::boardWidth - 2*globals::marginOffset, titleLabel->height() + 20);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->move((globals::boardWidth - titleLabel->width())/2, globals:: marginOffset);

    startButton = new QPushButton(this);
    startButton->setFont(createFont(48));
    startButton->setStyleSheet("QPushButton { background-color: yellow; color: black;}");
    startButton->setText("START");
    startButton->adjustSize();
    startButton->move((globals::boardWidth - startButton->width())/2, globals::boardHeight-globals::marginOffset-startButton->height());
}

WelcomeView::~WelcomeView(){
    delete titleLabel;
}