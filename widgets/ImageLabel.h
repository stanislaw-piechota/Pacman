#ifndef IMAGE_LABEL_H
#define IMAGE_LABEL_H
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include "../objects/EatableEntity.h"

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    ImageLabel() = default;
    ImageLabel(const char imagePath[], QWidget *parent = nullptr) : QLabel(parent)
    {
        image.load(imagePath);
        setPixmap(image);
        adjustSize();
        show();
    }
    ImageLabel(EatableEntity *eatable, QWidget *parent = nullptr) : QLabel(parent), eatable(eatable) {
        image = eatable->pixmap();
        setPixmap(image);
        adjustSize();
        show();
    }
    ~ImageLabel() = default;
    EatableEntity *eatable;

private:
    QPixmap image;
};

#endif