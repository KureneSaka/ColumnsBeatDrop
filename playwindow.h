#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include "playwindowwidgets.h"
#include "predefines.h"
#include "musicplayer.h"
#include "block.h"
#include <QSoundEffect>

enum playstatus {
    idle,
    normaldrop,
    trybeatdrop,
    bdsuccess,
    eliminating,
    to_eliminate,
    bdfinishing,
    gameover
};
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
    SoundPlayer cd1;
    SoundPlayer cd2;
    SoundPlayer fail;
    SoundPlayer beatdropsound;
    SoundPlayer eliminatesound1;
    SoundPlayer eliminatesound2;
    SoundPlayer shiftsound;
    bool countdowning = false;
    bool started = false;
    playstatus totalstatus = idle;
    int countdownnum = 0;
    int musicLength = 0;
    int loop = 0;
    int totalbeats = 0;
    int beatsperbar = 0;

    float downspeed = 0;
    bool speeduping = false;
    int speedupratio = 15;

    float beatdropspeed = 0;
    bool beatdropsuc = false;
    float beatdroptime = 0;
    int beatdropbeats = 0;


    column *droppingColumn = NULL;
    int droppingColumnX = 3;
    float droppingColumnY = 1.0;
    int droppingColumnYint = 1;
    bool droppingstop = false;
    int droppingstoptime = 0;
    int eliminatecount = 0;

    int falllines[BoardLines][BoardColumns] = {{0}};

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
    void NormalDrop(bool toEliminate = false);
    void GameOver();
    void TryBeatDrop();
    void BeatDropFail(float droptime);
    void BeatDropSuccess(float droptime);
    void Erase();
    void Eliminate();
signals:
    void playWindowUpdate();
public slots:

};

#endif // PLAYWINDOW_H
