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
    GrooveBar gb;
    RythmBar rb;
    NextBlockBoard nbb;
    ScoreBoard sb;
    MusicBoard mb;
    TimeBoard tb;
public:
    PlayWindow(QWidget *parent = nullptr);
    void start();
private:
    void paintEvent(QPaintEvent*);
    void shapeChange(State_w _stt);
//    void keyPressEvent(QKeyEvent *event);
signals:
    void playWindowUpdate();
public slots:

};

#endif // PLAYWINDOW_H
