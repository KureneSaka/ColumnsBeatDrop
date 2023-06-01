#ifndef PLAYWINDOWWIDGETS_H
#define PLAYWINDOWWIDGETS_H
#include <QWidget>
#include <QPainter>
#include <QFont>

class PlayWindowWidget: public QWidget
{
    Q_OBJECT;
protected:
    float showRatio = 0;
    void paintEvent(QPaintEvent *);
    virtual void Paint() = 0;
    void drawEdge(int x, int y, int w, int h, int shadowWidth = 6);
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
public:
    GrooveBar(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class RhythmBar: public PlayWindowWidget
{
    Q_OBJECT;
    int rhythmLevel = 20;
public:
    RhythmBar(QWidget *parent = nullptr);
    void flash();
    void upd();
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

class BottomRightBoard: public PlayWindowWidget
{
    Q_OBJECT;
    int beatsPerBar = 0;
    int bars = 0;
    int beats = 0;
    int totalbeats = 0;
    QFont WordFont;
    QFont DigitFont;
public:
    BottomRightBoard(QWidget *parent = nullptr);
    void init(int bpb);
    void setbeat(int _b);
private:
    void Paint();
signals:
public slots:
};

class BottomLeftBoard: public PlayWindowWidget
{
    Q_OBJECT;
    QFont WordFont;
    QFont DigitFont;
public:
    BottomLeftBoard(QWidget *parent = nullptr);
private:
    void Paint();
signals:
public slots:
};

class CountDown : public PlayWindowWidget
{
    Q_OBJECT;
    int cntdwn = 0;
    QFont _Font;

public:
    CountDown(QWidget *parent = nullptr);
    void setcntdwn(int cnt);
private:
    void Paint();
signals:
public slots:
};

#endif // PLAYWINDOWWIDGETS_H
