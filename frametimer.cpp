#include "frametimer.h"
#include "predefines.h"

FrameTimer::FrameTimer(QWidget *parent):QObject(parent)
{
    beginTime = new QDateTime(QDateTime::currentDateTime());
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[&](){
        QDateTime nowTime(QDateTime::currentDateTime());
        long long currentTime = beginTime->msecsTo(nowTime)*FPS/1000;
        if(currentTime!=frameTime)
        {
            emit frameRefresh();
            frameTime=currentTime;
        }
    });
    timer->start();
}
long long FrameTimer::getCurrentTime() const
{
    return frameTime;
}
FrameTimer::~FrameTimer()
{
    delete beginTime;
    delete timer;
}
