#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include "playwindowwidgets.h"
#include "predefines.h"
#include "musicplayer.h"
#include "block.h"
#include <QSoundEffect>

enum playstatus { idle, normaldrop, beating, beated, erasing, erased, dropping, dropped, gameover };
enum movedirection { left, right };

class PlayWindow:public QMainWindow
{
    Q_OBJECT;
    State_w stt;
    PlayBoard pb;
    RhythmBar rb;
    NextBlockBoard nbb;
    ScoreBoard sb;
    BottomRightBoard brb;
    BottomLeftBoard blb;
    GrooveBar gb;
    CountDown cntdwn;
    MusicPlayer music1;
    MusicPlayer music2;
    MusicPlayer cd1;
    MusicPlayer cd2;
    bool countdowning = false;
    bool started = false;
    playstatus totalstatus = idle;
    int countdownnum = 0;
    int musicLength = 0;
    int loop = 0;
    int totalbeats = 0;
    float downspeed = 0;

    column *droppingColumn = NULL;
    int droppingColumnX = 3;
    float droppingColumnY = 1.0;
    int droppingColumnYint = 1;
    bool droppingstop = false;
    int droppingstoptime = 0;

    bool speeduping = false;

    block *board[BoardLines][BoardColumns] = {{nullptr}};
public:
    PlayWindow(QWidget *parent = nullptr);
    ~PlayWindow();
    void Initialize();
    void setMusic(QString name);
private:
    void paintEvent(QPaintEvent *);
    void shapeChange(State_w _stt);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void refresh();
    void MoveColumn(movedirection dr);
    void UpShiftColumn(bool is_up);
    void ColumnNormalDrop();
    void GameOver();
//    void beatdrop();
signals:
    void playWindowUpdate();
public slots:

};

#endif // PLAYWINDOW_H
