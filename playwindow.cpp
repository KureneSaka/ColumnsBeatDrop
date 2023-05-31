#include "playwindow.h"
#include "frametimer.h"

extern FrameTimer *ftimer;

PlayWindow::PlayWindow(QWidget *parent)
    : QMainWindow(parent)
    , pb(this)
    , gb(this)
    , rb(this)
    , nbb(this)
    , sb(this)
    , mb(this)
    , tb(this)
{
    setWindowModality(Qt::NonModal);
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint);
    move(0, 0);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    stt = S_Hidden;
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

void PlayWindow::start()
{
    shapeChange(S_Expanding);
}
