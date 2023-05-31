#include "playwindow.h"
#include "frametimer.h"

extern FrameTimer *ftimer;
int grooveLevel = 0;
long long frameTimeOffset = 0;
int bpm = 134;
extern bool music_on;

PlayWindow::PlayWindow(QWidget *parent)
    : QMainWindow(parent)
    , pb(this)
    , rb(this)
    , nbb(this)
    , sb(this)
    , mb(this)
    , tb(this)
    , gb(this)
    , music1(this)
    , music2(this)
    , cd1(this)
    , cd2(this)
{
    music1.readMusic("TECHNOPOLIS 2085.mp3");
    music2.readMusic("TECHNOPOLIS 2085.mp3");
    cd1.readSound("countdown.wav");
    cd2.readSound("countdown_f.wav");
    musicLength = 130746;
    setWindowModality(Qt::NonModal);
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint);
    move(0, 0);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    stt = S_Hidden;
    connect(ftimer, &FrameTimer::frameRefresh, this, [=]() { refresh(); });
}

void PlayWindow::paintEvent(QPaintEvent *)
{
    if (stt == S_Hidden)
        return;
    long long frametime = ftimer->getCurrentTime();
    QPainter painter(this);
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
    mb.setRatio(shapeRatio);
    tb.setRatio(shapeRatio);
}

void PlayWindow::shapeChange(State_w _stt)
{
    switch (_stt) {
    case S_Narrowing:
        stt = _stt;
        frameTimeOffset = ftimer->getCurrentTime();
        setWindowModality(Qt::NonModal);
        clearFocus();
        break;
    case S_Expanding:
        stt = _stt;
        frameTimeOffset = ftimer->getCurrentTime();
        setWindowModality(Qt::ApplicationModal);
        setFocus();
        break;
    case S_Normal:
    case S_Hidden:
        break;
    }
}

void PlayWindow::GetIn()
{
    shapeChange(S_Expanding);
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
                if (countdownnum <= 3) {
                    cd1.cleanplay();
                } else if (countdownnum == 4) {
                    cd2.cleanplay();
                } else {
                    music1.play();
                    frameTimeOffset = ftimer->getCurrentTime();
                    started = true;
                    loop = 1;
                    music_on = true;
                }
                countdownnum++;
            }
            return;
        } else {
            long long currentTime = ftimer->getCurrentTime() - frameTimeOffset;
            long long loopTime = musicLength * loop * FPS / 1000;
            if (currentTime >= loopTime) {
                if (loop % 2 == 0) {
                    music1.play();
                } else {
                    music2.play();
                }
            }
        }
    }
}

void PlayWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat() == true)
        return;
    if (stt != S_Normal)
        return;
    switch (event->key()) {
    case Qt::Key_W:
        break;
    case Qt::Key_S:
        break;
    case Qt::Key_J:
        break;
    case Qt::Key_Up:
        grooveLevel++;
        break;
    case Qt::Key_Down:
        grooveLevel--;
        break;
    case Qt::Key_Return:
        break;
    }
}

