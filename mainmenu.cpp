#include "mainmenu.h"
#include "utils.h"
#include "frametimer.h"
#include <QDebug>
#include <QRawFont>
#include <QAudioDevice>
#include <QMediaDevices>

extern FrameTimer *ftimer;

MainMenu::MainMenu(QWidget *parent):QMainWindow(parent),select(this),confirm(this)
{
    select.setAudioDevice(QMediaDevices::defaultAudioOutput());
    confirm.setAudioDevice(QMediaDevices::defaultAudioOutput());
    select.setSource(QUrl::fromLocalFile("./res/sounds/select.wav"));
    confirm.setSource(QUrl::fromLocalFile("./res/sounds/confirm.wav"));
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);
    move(MIDDLE_WIDTH-350,MIDDLE_HEIGHT-350);
    resize(700,800);
    MenuFont=fontLoader("RationalInteger.ttf",40);
    TitleFont=fontLoader("STENCIL.TTF",100);
    setFocusPolicy(Qt::StrongFocus);
}

void MainMenu::paintEvent(QPaintEvent*)
{
    if(stt==S_Hidden)
        return;
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
        float x=350-220*shapeRatio+i*20;
        float y=537.5-202.5*shapeRatio+i*20;
        float w=440*shapeRatio-i*40;
        float h=405*shapeRatio-i*40;
        x=x>350?350:x;
        y=y>525?525:y;
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
        painter.drawText(180,400,340,50,Qt::AlignCenter,"S T A R T   G A M E");
        painter.drawText(180,475,340,50,Qt::AlignCenter,"S E T T I N G S");
        painter.drawText(180,550,340,50,Qt::AlignCenter,"H E L P");
        painter.drawText(180,625,340,50,Qt::AlignCenter,"Q U I T   G A M E");
        float brightness=40+200*abs(sin(qDegreesToRadians(frametime*5)));
        painter.setPen(QPen(QColor(brightness,brightness,brightness),3));
        painter.drawRect(190,390+75*cursor,320,70);
    }
    painter.setBrush(QBrush(QColor(50,50,50,127*shapeRatio),Qt::DiagCrossPattern));
    painter.setPen(QPen(Qt::transparent));
    painter.drawRect(50,10,600,300);

    painter.setBrush(QBrush(Qt::transparent));
    painter.setFont(TitleFont);
    painter.setPen(QPen(QColor(100,100,100,75*shapeRatio)));
    painter.drawText(15,15,680,300,Qt::AlignCenter,"COLUMNS\nBeat-Drop");
    painter.setPen(QPen(QColor(180+60*sin(qDegreesToRadians(frametime)),
                               180+60*sin(qDegreesToRadians(frametime+90)),
                               180+60*sin(qDegreesToRadians(frametime+180)),
                               255*shapeRatio)));
    painter.drawText(10,10,680,300,Qt::AlignCenter,"COLUMNS\nBeat-Drop");


}

void MainMenu::keyPressEvent(QKeyEvent *event)
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
        case C_MainMenu::Play:
            cursor = C_MainMenu::Quit;
            break;
        case C_MainMenu::Setting:
            cursor = C_MainMenu::Play;
            break;
        case C_MainMenu::Quit:
            cursor = C_MainMenu::Help;
            break;
        case C_MainMenu::Help:
            cursor = C_MainMenu::Setting;
            break;
        }
        select.play();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        switch(cursor)
        {
        case C_MainMenu::Play:
            cursor = C_MainMenu::Setting;
            break;
        case C_MainMenu::Setting:
            cursor = C_MainMenu::Help;
            break;
        case C_MainMenu::Help:
            cursor = C_MainMenu::Quit;
            break;
        case C_MainMenu::Quit:
            cursor = C_MainMenu::Play;
            break;
        }
        select.play();
        break;
    case Qt::Key_J:
    case Qt::Key_Return:
        switch(cursor)
        {

        case C_MainMenu::Play:
            shapeChange(S_Narrowing);
            emit StartGame();
            break;
        case C_MainMenu::Setting:
            break;
        case C_MainMenu::Help:
            break;
        case C_MainMenu::Quit:
            emit ExitGame();
            break;
        }
        confirm.play();
        break;
    }
}

void MainMenu::shapeChange(State_w _stt)
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

