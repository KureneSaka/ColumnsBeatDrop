#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include "playwindowwidgets.h"
#include "predefines.h"
#include "musicplayer.h"
#include <QSoundEffect>

class PlayWindow:public QMainWindow
{
    Q_OBJECT;
    State_w stt;
    PlayBoard pb;
    RhythmBar rb;
    NextBlockBoard nbb;
    ScoreBoard sb;
    MusicBoard mb;
    TimeBoard tb;
    GrooveBar gb;
    MusicPlayer music1;
    MusicPlayer music2;
    MusicPlayer cd1;
    MusicPlayer cd2;
    bool countdowning = false;
    bool started = false;
    int countdownnum = 1;
    int musicLength = 0;
    int loop = 0;
public:
    PlayWindow(QWidget *parent = nullptr);
    void GetIn();
    void setMusic(QString name);
private:
    void paintEvent(QPaintEvent *);
    void shapeChange(State_w _stt);
    void keyPressEvent(QKeyEvent *event);
    void refresh();
signals:
    void playWindowUpdate();
public slots:

};

#endif // PLAYWINDOW_H
