#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <QWidget>
#include <QPainter>

class BackGround: public QWidget
{
    Q_OBJECT;
    float angle;
public:
    BackGround(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
signals:
public slots:
};

#endif // BACKGROUND_H
