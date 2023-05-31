#ifndef PLAYWINDOWWIDGETS_H
#define PLAYWINDOWWIDGETS_H
#include <QWidget>
#include <QPainter>

class PlayWindowWidget: public QWidget
{
    Q_OBJECT;
protected:
    float showRatio=0;
public:
    PlayWindowWidget(QWidget *parent = nullptr);
    void setRatio(float shapeRatio);
private:
};

class PlayBoard: public PlayWindowWidget
{
    Q_OBJECT;
public:
    PlayBoard(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent*);
signals:
public slots:
};

class GrooveBar: public PlayWindowWidget
{
    Q_OBJECT;
public:
    GrooveBar(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent*);
signals:
public slots:
};

#endif // PLAYWINDOWWIDGETS_H
