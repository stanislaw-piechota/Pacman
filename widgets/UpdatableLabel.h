#ifndef UPDATABLE_LABEL_H
#define UPDATABLE_LABEL_H

#include <QLabel>
#include <QWidget>
#include <QFontDatabase>
#include <QString>

class UpdatableLabel : public QLabel {
    Q_OBJECT

public:
    UpdatableLabel(std::string base, const QFont &font, const char* color = "white", QWidget *parent = nullptr);
public slots:
    void update(const QString &string);
    void update(const char* string = "");
    void update(const int &number);
private:
    QString base;
};

#endif