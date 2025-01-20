#include "UpdatableLabel.h"
#include <string>

using namespace std;

UpdatableLabel::UpdatableLabel(string base, const QFont &font, const char color[], QWidget *parent) : QLabel(parent)
{
    this->base = QString::fromStdString(base);
    setFont(font);
    setStyleSheet("QLabel {color: " + QString::fromStdString(color) + ";}");
    update();
}

void UpdatableLabel::update(const QString &string)
{
    setText(base + string);
    adjustSize();
}

void UpdatableLabel::update(const char *string)
{
    update(QString::fromStdString(string));
}

void UpdatableLabel::update(const int &number)
{
    update(QString::number(number));
}