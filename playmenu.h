#ifndef PLAYMENU_H
#define PLAYMENU_H
#include "musicplayer.h"
#include "predefines.h"
#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QSoundEffect>
#include <QJsonObject>

class PlayMenu:public QMainWindow
{
    Q_OBJECT;
    QFont MenuFont;
    QFont ContentFont1;
    QFont ContentFont2;
    long long frameTimeOffset = 0;
    State_w stt = S_Hidden;
    int cursor = 0;
    int totalmusicnum = 0;
    SoundPlayer select;
    SoundPlayer confirm;
    QJsonObject musiclist;
    QImage *coverlist;
public:
    PlayMenu(QWidget *parent = nullptr);
    ~PlayMenu();
    int getMusicIndex();
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void shapeChange(State_w _stt);
signals:
    void GameStart();
public slots:

};

#endif // PLAYMENU_H
