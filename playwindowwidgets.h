#ifndef PLAYWINDOWWIDGETS_H
#define PLAYWINDOWWIDGETS_H
#include <QWidget>
#include <QPainter>
#include <QFont>

class PlayWindowWidget: public QWidget
{
    Q_OBJECT;
protected:
    float showRatio=0;
    void paintEvent(QPaintEvent*);
    virtual void Paint()=0;
    void drawEdge(int x,int y,int w,int h,int shadowWidth=6);
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
    void Paint();
signals:
public slots:
};

class GrooveBar: public PlayWindowWidget
{
    Q_OBJECT;
    QFont _Font;
    int grooveLevel;
public:
    GrooveBar(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class RythmBar: public PlayWindowWidget
{
    Q_OBJECT;
public:
    RythmBar(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class NextBlockBoard: public PlayWindowWidget
{
    Q_OBJECT;
    QFont _Font;
public:
    NextBlockBoard(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class ScoreBoard: public PlayWindowWidget
{
    Q_OBJECT;
    QFont WordFont;
    QFont DigitFont;
public:
    ScoreBoard(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class MusicBoard: public PlayWindowWidget
{
    Q_OBJECT;
    QFont WordFont;
    QFont DigitFont;
public:
    MusicBoard(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class TimeBoard: public PlayWindowWidget
{
    Q_OBJECT;
    QFont WordFont;
    QFont DigitFont;
public:
    TimeBoard(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};




#endif // PLAYWINDOWWIDGETS_H
