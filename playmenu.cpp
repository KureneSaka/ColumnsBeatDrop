#include "playmenu.h"
#include "frametimer.h"
#include "utils.h"
#include <QAudioDevice>
#include <QMediaDevices>

extern FrameTimer *ftimer;
PlayMenu::PlayMenu(QWidget *parent):QMainWindow(parent),select(this),confirm(this)
{
    select.setAudioDevice(QMediaDevices::defaultAudioOutput());
    confirm.setAudioDevice(QMediaDevices::defaultAudioOutput());
    select.setSource(QUrl::fromLocalFile("./res/sounds/select.wav"));
    confirm.setSource(QUrl::fromLocalFile("./res/sounds/confirm.wav"));
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);
    move(0,0);
    resize(WINDOW_WIDTH,WINDOW_HEIGHT);
    MenuFont=fontLoader("RationalInteger.ttf",40);
    ContentFont=fontLoader("UDDigiKyokashoN-R.ttc",40);
    setFocusPolicy(Qt::StrongFocus);
    connect(ftimer,&FrameTimer::frameRefresh,this,[=]()
            {
                if(stt!=S_Hidden)
                    update();
            });
    shapeChange(S_Expanding);
}

void PlayMenu::paintEvent(QPaintEvent*)
{
    long long frametime = ftimer->getCurrentTime();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QBrush(QColor(0,0,0,150)));
    float shapeRatio = 1;
    QRect rec[3];
    switch(stt)
    {
    case S_Expanding:
        shapeRatio = (frametime-frameTimeOffset)/20.0;
        shapeRatio = shapeRatio>1?1:shapeRatio;
        if(shapeRatio>=1)
        {
            stt=S_Normal;
            shapeRatio=1;
        }
        break;
    case S_Narrowing:
        shapeRatio = 1-(frametime-frameTimeOffset)/20.0;
        if(shapeRatio<=0)
        {
            stt=S_Hidden;
            shapeRatio=0;
        }
        break;
    case S_Normal:
        shapeRatio = 1;
        break;
    case S_Hidden:
        shapeRatio = 0;
        break;
    }
    for(int i = 0; i < 3; i++)
    {
        float x=MIDDLE_WIDTH-(MIDDLE_WIDTH-30)*shapeRatio+i*20;
        float y=MIDDLE_HEIGHT-(MIDDLE_HEIGHT-30)*shapeRatio+i*20;
        float w=(WINDOW_WIDTH-60)*shapeRatio-i*40;
        float h=(WINDOW_HEIGHT-60)*shapeRatio-i*40;
        x=x>MIDDLE_WIDTH?MIDDLE_WIDTH:x;
        y=y>MIDDLE_HEIGHT?MIDDLE_HEIGHT:y;
        w=w<0?0:w;
        h=h<0?0:h;
        rec[i]=QRect(x,y,w,h);
    }

    for(int i = 0; i < 3; i++)
    {
        QColor color=QColor(120+120*sin(qDegreesToRadians(frametime+i*30)),
                              120+120*sin(qDegreesToRadians(frametime+i*30+90)),
                              120+120*sin(qDegreesToRadians(frametime+i*30+180)));
        painter.setPen(QPen(color,5));
        painter.drawRect(rec[i]);
    }
    if(stt==S_Normal)
    {
        painter.setBrush(QBrush(Qt::transparent));
        painter.setPen(QPen(Qt::white));
        painter.setFont(MenuFont);
        painter.drawText(MIDDLE_WIDTH-100,WINDOW_HEIGHT-150,200,50,Qt::AlignCenter,"O K");
        float brightness=40+200*abs(sin(qDegreesToRadians(frametime*5)));
        painter.setPen(QPen(QColor(brightness,brightness,brightness),3));
        painter.drawRect(MIDDLE_WIDTH-100,WINDOW_HEIGHT-150,200,50);
    }
}
void PlayMenu::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()==true)
        return;
    if(stt!=S_Normal)
        return;
    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        switch(cursor)
        {
        }
        select.play();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        switch(cursor)
        {
        }
        select.play();
        break;
    case Qt::Key_J:
    case Qt::Key_Return:
        switch(cursor)
        {
        }
        shapeChange(S_Narrowing);
        confirm.play();
        emit GameStart();
        break;
    }
}

void PlayMenu::shapeChange(State_w _stt)
{
    switch(_stt)
    {
    case S_Narrowing:
        stt=_stt;
        frameTimeOffset=ftimer->getCurrentTime();
        setWindowModality(Qt::NonModal);
        clearFocus();
        break;
    case S_Expanding:
        stt=_stt;
        frameTimeOffset=ftimer->getCurrentTime();
        setWindowModality(Qt::ApplicationModal);
        setFocus();
        break;
    case S_Normal:
    case S_Hidden:
        break;
    }
}


