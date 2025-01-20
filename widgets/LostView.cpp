#include "LostView.h"
#include "globals.h"

using namespace globals;

LostView::LostView() {
    lostLabel = new UpdatableLabel("YOU\nLOST\n\n", createFont(36), "white", this);
    lostLabel->update(0);
    lostLabel->setFixedSize(globals::boardWidth, globals::boardHeight);
    lostLabel->setAlignment(Qt::AlignCenter);
    setFixedSize(globals::boardWidth, globals::boardHeight);
    lostLabel->move(0, 0);
    setStyleSheet("background-color: black");
}

LostView::~LostView(){
    delete lostLabel;
}

void LostView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        exit(0);
}