#include <QWidget>
#include "UpdatableLabel.h"
#include <QKeyEvent>

class LostView: public QWidget {
    Q_OBJECT

public:
    LostView();
    ~LostView();
    UpdatableLabel *lostLabel;
protected:
    void keyPressEvent(QKeyEvent *event) override;
};