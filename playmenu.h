#ifndef PLAYMENU_H
#define PLAYMENU_H
#include "predefines.h"
#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QSoundEffect>

class PlayMenu:public QMainWindow
{
    Q_OBJECT;
    QFont MenuFont;
    QFont ContentFont;
    long long frameTimeOffset = 0;
    State_w stt = S_Hidden;
    int cursor = 0;
    QSoundEffect select;
    QSoundEffect confirm;
public:
    PlayMenu(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void shapeChange(State_w _stt);
signals:
    void GameStart();
public slots:

};

#endif // PLAYMENU_H
