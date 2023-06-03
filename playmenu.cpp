#include "playmenu.h"
#include "frametimer.h"
#include "utils.h"
#include <QAudioDevice>
#include <QMediaDevices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

extern FrameTimer *ftimer;
int selectedMusic = -1;
PlayMenu::PlayMenu(QWidget *parent)
    : QMainWindow(parent)
    , select(this)
    , confirm(this)
{
    QFile file("./res/music.json");
    file.open(QIODevice::ReadOnly);
    QByteArray data(file.readAll());
    file.close();
    QJsonParseError jerr;
    QJsonDocument jdoc = QJsonDocument::fromJson(data, &jerr);
    musiclist = jdoc.object();
    totalmusicnum = musiclist["musiclist"].toArray().size();
    coverlist = new QImage[totalmusicnum];
    for (int i = 0; i < totalmusicnum; i++) {
        coverlist[i].load("./res/pics/" + musiclist["musiclist"][i]["name"].toString() + ".png");
    }

    select.readSound("select.wav");
    confirm.readSound("confirm.wav");

    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);
    move(0, 0);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    MenuFont = fontLoader("STENCIL.TTF", 40);
    ContentFont1 = fontLoader("UDDigiKyokashoN-R.ttc", 40);
    ContentFont2 = fontLoader("UDDigiKyokashoN-R.ttc", 25);
    setFocusPolicy(Qt::StrongFocus);
    connect(ftimer, &FrameTimer::frameRefresh, this, [=]() {
        if (stt != S_Hidden)
            update();
    });
    shapeChange(S_Expanding);
}
PlayMenu::~PlayMenu()
{
    delete[] coverlist;
}

void PlayMenu::paintEvent(QPaintEvent *)
{
    if (stt == S_Hidden)
        return;
    long long frametime = ftimer->getCurrentTime();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(0, 0, 0, 150)));
    float shapeRatio = 1;
    QRect rec[3];
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
    for (int i = 0; i < 3; i++) {
        float x = MIDDLE_WIDTH - (MIDDLE_WIDTH - 30) * shapeRatio + i * 20;
        float y = MIDDLE_HEIGHT - (MIDDLE_HEIGHT - 30) * shapeRatio + i * 20;
        float w = (WINDOW_WIDTH - 60) * shapeRatio - i * 40;
        float h = (WINDOW_HEIGHT - 60) * shapeRatio - i * 40;
        x = x > MIDDLE_WIDTH ? MIDDLE_WIDTH : x;
        y = y > MIDDLE_HEIGHT ? MIDDLE_HEIGHT : y;
        w = w < 0 ? 0 : w;
        h = h < 0 ? 0 : h;
        rec[i] = QRect(x, y, w, h);
    }

    for (int i = 0; i < 3; i++) {
        QColor color = QColor(120 + 120 * sin(qDegreesToRadians(frametime + i * 30)),
                              120 + 120 * sin(qDegreesToRadians(frametime + i * 30 + 90)),
                              120 + 120 * sin(qDegreesToRadians(frametime + i * 30 + 180)));
        painter.setPen(QPen(color, 5));
        painter.drawRect(rec[i]);
    }
    if (stt == S_Normal) {
        painter.setBrush(QBrush(Qt::transparent));
        painter.setPen(QPen(QColor(240, 240, 120)));
        painter.setFont(MenuFont);
        painter.drawText(100, 100, WINDOW_WIDTH - 200, 50, Qt::AlignCenter, "SELECT MUSIC");

        float brightness = 40 + 200 * abs(sin(qDegreesToRadians(frametime * 5)));
        painter.setPen(QPen(QColor(brightness, brightness, brightness), 3));
        painter.drawRect(100, 200 + cursor * 170, WINDOW_WIDTH - 200, 150);


        for (int i = 0; i < totalmusicnum; i++) {
            QTransform tsfm;
            painter.setTransform(tsfm);
            painter.setPen(QPen(QColor(240, 240, 120)));
            painter.setFont(ContentFont1);
            painter.drawText(290,
                             200 + i * 170,
                             WINDOW_WIDTH - 200,
                             100,
                             Qt::AlignLeft | Qt::AlignVCenter,
                             musiclist["musiclist"][i]["name"].toString());
            painter.setPen(QPen(QColor(120, 120, 60)));
            painter.setFont(ContentFont2);
            painter.drawText(290,
                             265 + i * 170,
                             WINDOW_WIDTH - 200,
                             60,
                             Qt::AlignLeft | Qt::AlignVCenter,
                             musiclist["musiclist"][i]["artist"].toString());

            QString str = "";
            str += "   BPM  ";
            str += std::to_string(musiclist["musiclist"][i]["bpm"].toInt());
            int tm = musiclist["musiclist"][i]["length"].toInt();
            str += "\nLENGTH  ";
            str += std::to_string(tm / 60000);
            str += ":";
            if (tm % 60000 / 10000 == 0) {
                str += "0";
            }
            str += std::to_string(tm % 60000 / 1000);

            painter.drawText(800,
                             200 + i * 170,
                             WINDOW_WIDTH - 200,
                             150,
                             Qt::AlignLeft | Qt::AlignVCenter,
                             str);


            tsfm.translate(100 + 30, 200 + i * 170+10);
            tsfm.scale(130.0 / coverlist[i].width(), 130.0 / coverlist[i].width());
            painter.setTransform(tsfm);
            painter.drawImage(0, 0, coverlist[i]);
        }
    }
}
void PlayMenu::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat() == true)
        return;
    if (stt != S_Normal)
        return;
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        cursor--;
        cursor = cursor < 0 ? totalmusicnum - 1 : cursor;
        select.play();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        cursor++;
        cursor = cursor >= totalmusicnum ? 0 : cursor;
        select.play();
        break;
    case Qt::Key_J:
    case Qt::Key_Return:
        switch (cursor) {}
        shapeChange(S_Narrowing);
        confirm.play();
        emit GameStart();
        break;
    }
}

void PlayMenu::shapeChange(State_w _stt)
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

int PlayMenu::getMusicIndex()
{
    return cursor;
}
