#include <QWidget>
#include <QPushButton>
#include "UpdatableLabel.h"
#include "ImageLabel.h"
#include "../objects/Entity.h"

class WelcomeView: public QWidget {
    Q_OBJECT

public:
    WelcomeView();
    ~WelcomeView();
    UpdatableLabel *titleLabel;
    QPushButton *startButton;
};