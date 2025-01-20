#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QFontDatabase>
#include "UpdatableLabel.h"
#include "ImageLabel.h"
#include "objects/Board.h"

class MainView : public QWidget
{
    Q_OBJECT
public:
    MainView(QWidget *parent = nullptr);
    ~MainView();
    inline Board *getBoard() { return board; }
signals:
    void gameLost(int points);
private slots:
    void updateLives(int left);
    void createFruitLabel(EatableEntity* eatable);
private:
    Board *board;
    UpdatableLabel *pointsLabel, *readyLabel, *levelLabel;
    ImageLabel *lives[globals::numberOfLives];
    QList<ImageLabel *> fruits;
    void displayFruits();
    void removeFruit(EatableEntity *eatable);
    void createLivesLabels();
    void setupWidgets();
    void setupConnections();
};

#endif
