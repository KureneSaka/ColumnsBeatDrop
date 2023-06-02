#include "playwindow.h"
#include "frametimer.h"

extern FrameTimer *ftimer;
extern int selectedMusic;
int grooveLevel = 0;
long long frameTimeOffset = 0;
long long realTimeOffset = 0;//in ms
int bpm = 0;
extern bool music_on;
double beattime = 0;//in ms
const int SquareWidth = 50;
const int BoardY = 80;
const int BoardX0 = (WINDOW_WIDTH - BoardColumns * SquareWidth) / 2;

PlayWindow::PlayWindow(QWidget *parent)
    : QMainWindow(parent)
    , pb(this)
    , rb(this)
    , nbb(this)
    , sb(this)
    , brb(this)
    , blb(this)
    , gb(this)
    , cntdwn(this)
    , music1(this)
    , music2(this)
    , cd1(this)
    , cd2(this)
    , fail(this)
    , beatdropsound(this)
    , eliminatesound(this)
    , shiftsound(this)
{
    cd1.readSound("countdown.wav");
    cd2.readSound("countdown_f.wav");
    fail.readSound("fail.wav");
    beatdropsound.readSound("beatdrop.wav");
    eliminatesound.readSound("eliminate.wav");
    shiftsound.readSound("shift.wav");
    setWindowModality(Qt::NonModal);
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint);
    move(0, 0);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    stt = S_Hidden;
    connect(ftimer, &FrameTimer::frameRefresh, this, [=]() { refresh(); });
}
PlayWindow::~PlayWindow()
{
    delete droppingColumn;
    for (int i = 0; i < BoardLines; i++) {
        for (int j = 0; j < BoardColumns; j++) {
            if (board[i][j])
                delete board[i][j];
        }
    }
}

void PlayWindow::paintEvent(QPaintEvent *)
{
    if (stt == S_Hidden)
        return;
    long long frametime = ftimer->getCurrentTime();
    float shapeRatio = 1;
    switch (stt) {
    case S_Expanding:
        shapeRatio = (frametime - frameTimeOffset) / 20.0;
        shapeRatio = shapeRatio > 1 ? 1 : shapeRatio;
        if (shapeRatio >= 1) {
            stt = S_Normal;
            shapeRatio = 1;
        }
        break;
    case S_Narrowing:
        shapeRatio = 1 - (frametime - frameTimeOffset) / 20.0;
        if (shapeRatio <= 0) {
            stt = S_Hidden;
            shapeRatio = 0;
        }
        break;
    case S_Normal:
        shapeRatio = 1;
        break;
    case S_Hidden:
        shapeRatio = 0;
        break;
    }
    pb.setRatio(shapeRatio);
    gb.setRatio(shapeRatio);
    rb.setRatio(shapeRatio);
    nbb.setRatio(shapeRatio);
    sb.setRatio(shapeRatio);
    brb.setRatio(shapeRatio);
    blb.setRatio(shapeRatio);
    cntdwn.setcntdwn(countdownnum);
    if (droppingColumn) {
        droppingColumn->move(BoardX0 + droppingColumnX * SquareWidth,
                             BoardY + (droppingColumnY - 3) * SquareWidth);
    }
}

void PlayWindow::shapeChange(State_w _stt)
{
    switch (_stt) {
    case S_Narrowing:
        stt = _stt;
        frameTimeOffset = ftimer->getCurrentTime();
        setWindowModality(Qt::NonModal);
        setFocusPolicy(Qt::NoFocus);
        clearFocus();
        break;
    case S_Expanding:
        stt = _stt;
        frameTimeOffset = ftimer->getCurrentTime();
        setWindowModality(Qt::ApplicationModal);
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
        break;
    case S_Normal:
    case S_Hidden:
        break;
    }
}

void PlayWindow::Initialize()
{
    shapeChange(S_Expanding);
    QString musicname = "1.mp3";
    music1.readMusic(musicname);
    music2.readMusic(musicname);

    bpm = 134;
    beattime = double(60000) / bpm;
    //musicLength = 130746;
    blb.loadcover("TECHNOPOLIS 2085.png");
    blb.setsong("TECHNOPOLIS 2085", "PRASTIK DANCEFLOOR");
    musicLength = 10746;
    brb.init(4);
    downspeed = 0.5 * bpm / 3600;
}

void PlayWindow::refresh()
{
    if (stt == S_Normal) {
        if (!countdowning && !started) {
            frameTimeOffset = ftimer->getCurrentTime();
            countdowning = true;
            return;
        }
        if (!started) {
            long long currentTime = ftimer->getCurrentTime() - frameTimeOffset;
            if (currentTime < 60)
                return;
            currentTime -= 60;
            if (currentTime >= 3600 * countdownnum / bpm) {
                nbb.initialnew(countdownnum);
                if (countdownnum <= 2) {
                    cd1.play();
                } else if (countdownnum == 3) {
                    cd2.play();
                } else {
                    music1.play();
                    realTimeOffset = ftimer->getRealTime();
                    frameTimeOffset = ftimer->getCurrentTime();
                    started = true;
                    music_on = true;
                    countdowning = false;
                }
                countdownnum++;
            }
            return;
        } else {
            if (board[0][3] && totalstatus == idle) {
                GameOver();
            }
            long long currentTime = ftimer->getCurrentTime() - frameTimeOffset;
            long long loopTime = musicLength * (loop+1) * FPS / 1000;
            long long beatsTime = beattime * totalbeats * FPS / 1000;
            float deltaTime = currentTime * float(bpm) / FPS / 60 + 1
                              - totalbeats; //0 when just on beat, 1 when next beat
            long long currentrealtime = ftimer->getRealTime() - realTimeOffset;
            if (currentTime >= loopTime && totalstatus != gameover) {
                loop++;
                if (loop % 2 == 0) {
                    music1.play();
                } else {
                    music2.play();
                }
            }
            if (currentTime >= beatsTime) {
                totalbeats++;
                brb.setbeat(totalbeats);
                if (droppingColumn == NULL && totalstatus == idle) {
                    rb.finishflash();
                    gb.finishflash();
                    droppingColumnX = 3;
                    droppingColumnY = 1.0;
                    droppingColumnYint = 1;
                    droppingColumn = nbb.popColumn();
                }
                if (totalstatus == bdsuccess) {
                    shapechangetime = currentTime;
                    totalstatus = eliminating;
                    Eliminate();
                }
            }
            switch (totalstatus) {
            case idle: {
                if (droppingColumn) {
                    droppingColumn->setshine(cos(qDegreesToRadians(deltaTime * 90)));
                    if (speeduping)
                        droppingColumnY += 10 * downspeed;
                    else
                        droppingColumnY += downspeed;
                    if (droppingColumnY >= droppingColumnYint) {
                        if (droppingColumnYint == BoardLines
                            || board[droppingColumnYint][droppingColumnX]) {
                            droppingColumnY = droppingColumnYint;
                            if (droppingstop == false) {
                                droppingstop = true;
                                droppingstoptime = 0;
                            }
                        } else {
                            droppingColumnYint++;
                            if (droppingstop == true)
                                droppingstop = false;
                        }
                        if (droppingstop) {
                            if (speeduping)
                                droppingstoptime += 5;
                            else
                                droppingstoptime++;
                            if (droppingstoptime >= beattime * 60 * 2 / 1000) {
                                totalstatus = normaldrop;
                                NormalDrop();
                                totalstatus = idle;
                                droppingstop = false;
                            }
                        }
                    }
                }
                rb.upd(currentrealtime);
            } break;
            case trybeatdrop: {
                if (droppingColumn) {
                    droppingColumn->setshine(1);
                    droppingColumnY += beatdropspeed;
                    if (droppingColumnY >= droppingColumnYint) {
                        if (droppingColumnYint == BoardLines
                            || board[droppingColumnYint][droppingColumnX]) {
                            droppingColumnY = droppingColumnYint;
                            if (beatdropsuc) {
                                NormalDrop(true);
                                totalstatus = bdsuccess;
                            } else if (!beatdropsuc) {
                                NormalDrop();
                                totalstatus = idle;
                            }
                        } else {
                            droppingColumnYint++;
                        }
                    }
                }
                rb.upd(beatdroptime);
            } break;
            case bdsuccess:
                rb.upd(beatdroptime);
                break;
            case gameover:
                break;
            default:
                rb.upd(currentrealtime);
                break;
            }
        }
    }
}

void PlayWindow::MoveColumn(movedirection dr)
{
    if (!droppingColumn || totalstatus != idle) {
        return;
    }
    switch (dr) {
    case left:
        if (board[droppingColumnYint - 1][droppingColumnX - 1]) {
            return;
        } else {
            droppingColumnX--;
            droppingColumnX = droppingColumnX >= 0 ? droppingColumnX : 0;
        }
        break;
    case right:
        if (board[droppingColumnYint - 1][droppingColumnX + 1]) {
            return;
        } else {
            droppingColumnX++;
            droppingColumnX = droppingColumnX < BoardColumns ? droppingColumnX : BoardColumns - 1;
        }
        break;
    }
}
void PlayWindow::UpShiftColumn(bool is_up)
{
    shiftsound.play();
    if (!droppingColumn || totalstatus != idle) {
        return;
    }
    droppingColumn->exchange(is_up);
}

void PlayWindow::NormalDrop(bool toEliminate)
{
    for (int i = 0; i < 3; i++) {
        if (droppingColumnYint - i - 1 >= 0) {
            board[droppingColumnYint - i - 1][droppingColumnX]
                = new block(this, droppingColumn->getBlockColor(2 - i));
            board[droppingColumnYint - i - 1][droppingColumnX]
                ->move(BoardX0 + droppingColumnX * SquareWidth,
                       BoardY + (droppingColumnYint - i - 1) * SquareWidth);
            if (toEliminate) {
                board[droppingColumnYint - i - 1][droppingColumnX]->setEliminate(true);
            }
            board[droppingColumnYint - i - 1][droppingColumnX]->show();
        }
    }
    column *tmp = droppingColumn;
    droppingColumn = nullptr;
    delete tmp;
    totalstatus = idle;
}

void PlayWindow::TryBeatDrop()
{
    droppingstop = false;
    beatdroptime = ftimer->getRealTime() - realTimeOffset;
    float deltaTime = beatdroptime - (totalbeats - 1) * beattime;
    float beatableTime = beattime / 40 * (10 - (grooveLevel > 9 ? 9 : grooveLevel));
    if ((deltaTime >= -beatableTime && deltaTime <= beatableTime)
        || (deltaTime >= beattime - beatableTime && deltaTime <= beattime + beatableTime)) {
        beatdropsuc = true;
        beatdropsound.play();
        gb.startflash();
        rb.startflash();
        float droptime = 0;
        if (deltaTime > -beatableTime && deltaTime < beatableTime) {
            droptime = beattime * 2 - deltaTime;
        } else {
            droptime = beattime * 3 - deltaTime;
        }
        BeatDropSuccess(droptime);
    } else {
        beatdropsuc = false;
        float droptime = beattime * 2;
        BeatDropFail(droptime);
    }
}
void PlayWindow::BeatDropSuccess(float droptime)
{
    int destY = 0;
    for (; board[destY][droppingColumnX] == NULL; destY++) {
    }
    if (destY - droppingColumnY < 0.03) {
        beatdropspeed = 1 / droptime * SquareWidth;
    } else {
        beatdropspeed = (destY - droppingColumnY) / droptime * SquareWidth;
    }
}
void PlayWindow::BeatDropFail(float droptime)
{
    int destY = 0;
    for (; board[destY][droppingColumnX] == NULL; destY++) {
    }
    if (destY - droppingColumnY < 0.03) {
        beatdropspeed = 1 / droptime * SquareWidth;
    } else {
        beatdropspeed = (destY - droppingColumnY) / droptime * SquareWidth;
    }
}

void PlayWindow::Eliminate()
{
    /*
    int groupnum[4] = {1, 1, 1, 1};
    QMap<int, QPair<QList<block *>, bool>> map[4];
    for (int i = 0; i < BoardLines; i++) {
        for (int j = 0; j < BoardColumns; j++) {
            if (board[i][j]) {
                blockColor Mclr = board[i][j]->getBlockColor();
                blockColor Uclr = BNULL;
                blockColor Lclr = BNULL;
                int Mgrp = 0;
                int Ugrp = 0;
                int Lgrp = 0;
                if (i > 0 && board[i - 1][j]) {
                    Uclr = board[i - 1][j]->getBlockColor();
                    Ugrp = board[i - 1][j]->getEliminateGroup();
                }
                if (j > 0 && board[i][j - 1]) {
                    Lclr = board[i][j - 1]->getBlockColor();
                    Lgrp = board[i][j - 1]->getEliminateGroup();
                }
                if (Mclr != Uclr && Mclr != Lclr) {
                    map[Mclr].insert(groupnum[Mclr],
                                     QPair<QList<block *>, bool>(QList<block *>{board[i][j]},
                                                                 board[i][j]->getEliminate()));
                    board[i][j]->setEliminateGroup(groupnum[Mclr]);
                    groupnum[Mclr]++;
                    continue;
                }
                if (Mclr == Uclr && Mclr == Lclr) {
                    Mgrp = Ugrp;
                    if (Lgrp == Ugrp) {
                    }
                    if (Lgrp != Ugrp) {
                        map[Mclr][Mgrp].first.append(map[Mclr][Lgrp].first);
                        for (int i = map[Mclr][Lgrp].first.size() - 1; i >= 0; i--) {
                            map[Mclr][Lgrp].first[i]->setEliminateGroup(Mgrp);
                        }
                        map[Mclr].remove(Lgrp);
                    }
                } else if (Mclr == Uclr) {
                    Mgrp = Ugrp;
                } else if (Mclr == Lclr) {
                    Mgrp = Lgrp;
                }
                board[i][j]->setEliminateGroup(Mgrp);
                map[Mclr][Mgrp].first.push_back(board[i][j]);
                if (board[i][j]->getEliminate()) {
                    map[Mclr][Mgrp].second = true;
                }
            }
        }
    }
    totalstatus = idle;*/
}
void PlayWindow::Fall()
{

}


void PlayWindow::GameOver()
{
    totalstatus = gameover;
    music1.stop();
    music2.stop();
    music_on = false;
    fail.play();
    pb.GameOver();
    rb.GameOver();
    nbb.GameOver();
    sb.GameOver();
    brb.GameOver();
    blb.GameOver();
    gb.GameOver();
    cntdwn.GameOver();

}

void PlayWindow::keyPressEvent(QKeyEvent *event)
{
    if (stt != S_Normal)
        return;
    if (!started)
        return;
    switch (event->key()) {
    case Qt::Key_Down:
    case Qt::Key_S:
        if (event->isAutoRepeat() == true)
            return;
        speeduping = true;
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        MoveColumn(left);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        MoveColumn(right);
        break;
    case Qt::Key_J:
    case Qt::Key_Z:
        if (event->isAutoRepeat() == true)
            return;
        UpShiftColumn(false);
        break;
    case Qt::Key_K:
    case Qt::Key_X:
        if (event->isAutoRepeat() == true)
            return;
        UpShiftColumn(true);
        break;
    case Qt::Key_Space:
        if (event->isAutoRepeat() == true)
            return;
        if (totalstatus == idle && droppingColumn) {
            totalstatus = trybeatdrop;
            TryBeatDrop();
        }
        break;
    case Qt::Key_1:
        grooveLevel++;
        grooveLevel = grooveLevel >= 10 ? 10 : grooveLevel;
        break;
    case Qt::Key_2:
        grooveLevel--;
        grooveLevel = grooveLevel <= 0 ? 0 : grooveLevel;
        break;
    }
}

void PlayWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat() == true)
        return;
    if (stt != S_Normal)
        return;
    if (!started)
        return;
    switch (event->key()) {
    case Qt::Key_Down:
    case Qt::Key_S:
        speeduping = false;
        break;
    }
}


