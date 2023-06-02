#include "playwindow.h"
#include "frametimer.h"
#include "utils.h"

extern FrameTimer *ftimer;
extern int selectedMusic;
int grooveLevel = 0;
long long frameTimeOffset = 0;
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
{
    cd1.readSound("countdown.wav");
    cd2.readSound("countdown_f.wav");
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
    for (int i = BoardLines - BoardColumns; i < BoardLines; i++) {
        for (int j = 0; j < i - BoardLines + BoardColumns; j++) {
            board[i][j] = new block(this, BNULL);
            board[i][j]->move(BoardX0 + j * SquareWidth, BoardY + i * SquareWidth);
            board[i][j]->show();
        }
    }
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
                    cd1.cleanplay();
                } else if (countdownnum == 3) {
                    cd2.cleanplay();
                } else {
                    music1.play();
                    frameTimeOffset = ftimer->getCurrentTime();
                    started = true;
                    music_on = true;
                    countdowning = false;
                }
                countdownnum++;
            }
            return;
        } else {
            long long currentTime = ftimer->getCurrentTime() - frameTimeOffset;
            long long loopTime = musicLength * (loop+1) * FPS / 1000;
            long long beatsTime = beattime * totalbeats * FPS / 1000;
            float deltaTime = currentTime * float(bpm) / FPS / 60 + 1
                              - totalbeats; //0 when just on beat, 1 when next beat
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
                    if (board[0][3]) {
                        GameOver();
                    } else {
                        droppingColumnX = 3;
                        droppingColumnY = 1.0;
                        droppingColumnYint = 1;
                        droppingColumn = nbb.popColumn();
                    }
                }
            }
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
                            ColumnNormalDrop();
                            droppingstop = false;
                        }
                    }
                }
            }
            rb.upd();
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
    if (!droppingColumn || totalstatus != idle) {
        return;
    }
    droppingColumn->exchange(is_up);
}

void PlayWindow::ColumnNormalDrop()
{
    for (int i = 0; i < 3; i++) {
        if (droppingColumnYint - i - 1 >= 0) {
            board[droppingColumnYint - i - 1][droppingColumnX]
                = new block(this, droppingColumn->getBlockColor(2 - i));
            board[droppingColumnYint - i - 1][droppingColumnX]
                ->move(BoardX0 + droppingColumnX * SquareWidth,
                       BoardY + (droppingColumnYint - i - 1) * SquareWidth);
            board[droppingColumnYint - i - 1][droppingColumnX]->show();
        }
    }
    column *tmp = droppingColumn;
    droppingColumn = NULL;
    delete tmp;
    totalstatus = idle;
}

void PlayWindow::GameOver()
{
    totalstatus = gameover;
    music1.stop();
    music2.stop();
    music_on = false;
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
    if (event->isAutoRepeat() == true)
        return;
    if (stt != S_Normal)
        return;
    if (!started)
        return;
    switch (event->key()) {
    case Qt::Key_Down:
    case Qt::Key_S:
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
        UpShiftColumn(false);
        break;
    case Qt::Key_K:
    case Qt::Key_X:
        UpShiftColumn(true);
        break;
    case Qt::Key_Space:
        if (totalstatus == idle) {
            totalstatus = beating;
            //beatdrop();
        }
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


