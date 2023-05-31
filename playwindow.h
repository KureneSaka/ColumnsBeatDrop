#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H
#include <QMainWindow>
#include "playwindowwidgets.h"
#include "predefines.h"

class PlayWindow:public QMainWindow
{
    Q_OBJECT;
    State_w stt;
    long long frameTimeOffset;
    PlayBoard pb;
    float shapeRatio = 0;
public:
    PlayWindow(QWidget *parent = nullptr);
    void start();
    float getShapeRatio();
private:
    void paintEvent(QPaintEvent*);
    void shapeChange(State_w _stt);
//    void keyPressEvent(QKeyEvent *event);
signals:
    void playWindowUpdate();
public slots:

};

#endif // PLAYWINDOW_H
