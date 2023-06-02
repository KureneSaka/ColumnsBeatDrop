#ifndef INNERTIMER_H
#define INNERTIMER_H
#include<QWidget>
#include<QTimer>
#include<QDateTime>

class FrameTimer:public QObject
{
    Q_OBJECT;
    long long frameTime = 0;
    QTimer *timer;
    QDateTime *beginTime;
public:
    FrameTimer(QWidget *parent = nullptr);
    ~FrameTimer();
    long long getCurrentTime() const;
    long long getRealTime() const;
signals:
    void frameRefresh();
public slots:
};

#endif // INNERTIMER_H
