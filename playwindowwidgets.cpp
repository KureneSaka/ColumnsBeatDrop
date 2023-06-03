#include "playwindowwidgets.h"
#include "frametimer.h"
#include "predefines.h"
#include "utils.h"
#include <QRandomGenerator>

extern const int SquareWidth = 50;
const int BoardWidth = BoardColumns * SquareWidth;
const int BoardHeight = BoardLines * SquareWidth;
extern const int BoardY = 80;
const int BoardPenWidth = 4;
extern int grooveLevel;
extern long long frameTimeOffset;
extern int bpm;
extern FrameTimer *ftimer;

QColor GrooveColor[10] = {QColor(240, 0, 0),
                          QColor(240, 160, 0),
                          QColor(240, 240, 0),
                          QColor(160, 240, 0),
                          QColor(0, 240, 0),
                          QColor(0, 240, 160),
                          QColor(0, 240, 200),
                          QColor(0, 240, 240),
                          QColor(0, 200, 240),
                          QColor(0, 160, 240)};
QString RatioString[10]
    = {"20.0X", "10.0X", "6.00X", "4.00X", "3.20X", "2.40X", "2.00X", "1.60X", "1.25X", "1.00X"};
float Ratio[10] = {20.0, 10.0, 6.00, 4.00, 3.20, 2.40, 2.00, 1.60, 1.25, 1.00};

PlayWindowWidget::PlayWindowWidget(QWidget *parent)
    : QWidget(parent)
{
    move(0, 0);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    show();
}
void PlayWindowWidget::setRatio(float shapeRatio)
{
    showRatio = shapeRatio;
}
void PlayWindowWidget::paintEvent(QPaintEvent *)
{
    if (showRatio <= 0.01)
        return;
    Paint();
}
void PlayWindowWidget::drawEdge(int x, int y, int w, int h, int shadowWidth)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QColor(0, 200, 0, 255 * showRatio), BoardPenWidth));
    painter.setBrush(QBrush(QColor(0, 0, 0, 200 * showRatio)));
    painter.drawRect(x - BoardPenWidth / 2,
                     y - BoardPenWidth / 2,
                     w + BoardPenWidth,
                     h + BoardPenWidth);
    painter.setPen(QPen(QColor(0, 127, 0, 128 * showRatio), shadowWidth));
    QPoint pru(x + w + BoardPenWidth + shadowWidth / 2, y - BoardPenWidth / 2 + shadowWidth * 3 / 2);
    QPoint prd1(x + w + BoardPenWidth + shadowWidth / 2, y + h + BoardPenWidth - shadowWidth / 2);
    QPoint prd2(x + w + BoardPenWidth + shadowWidth / 2, y + h + BoardPenWidth + shadowWidth / 2);
    QPoint pld(x - BoardPenWidth / 2 + shadowWidth * 3 / 2, y + h + BoardPenWidth + shadowWidth / 2);
    painter.drawLine(pru, prd1);
    painter.drawLine(pld, prd2);
}
void PlayWindowWidget::GameOver()
{
    stop = true;
}

PlayBoard::PlayBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{}
void PlayBoard::Paint()
{
    int BoardX = (WINDOW_WIDTH - BoardWidth) / 2;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::transparent));
    painter.setBrush(QBrush(QColor(0, 0, 0, 240 * showRatio)));
    painter.drawRect(BoardX, BoardY, BoardWidth, BoardHeight);
    painter.setPen(QPen(QColor(0, 127, 0, 63 * showRatio), 1));
    for (int i = 1; i < BoardLines; i++) {
        painter.drawLine(BoardX,
                         BoardY + i * SquareWidth,
                         BoardX + BoardWidth,
                         BoardY + i * SquareWidth);
    }
    for (int i = 1; i < BoardColumns; i++) {
        painter.drawLine(BoardX + i * SquareWidth,
                         BoardY,
                         BoardX + i * SquareWidth,
                         BoardY + BoardHeight);
    }
    painter.setPen(QPen(QColor(0, 200, 0, 255 * showRatio), BoardPenWidth));
    painter.setBrush(QBrush(Qt::transparent));
    painter.drawRect(BoardX - BoardPenWidth / 2,
                     BoardY - BoardPenWidth / 2,
                     BoardWidth + BoardPenWidth,
                     BoardHeight + BoardPenWidth);
    int shadowWidth = 8;
    painter.setPen(QPen(QColor(0, 127, 0, 128 * showRatio), shadowWidth));
    painter.drawLine(BoardX + BoardWidth + BoardPenWidth + shadowWidth / 2,
                     BoardY - BoardPenWidth / 2 + shadowWidth * 3 / 2,
                     BoardX + BoardWidth + BoardPenWidth + shadowWidth / 2,
                     BoardY + BoardHeight + BoardPenWidth - shadowWidth / 2);
    painter.drawLine(BoardX - BoardPenWidth / 2 + shadowWidth * 3 / 2,
                     BoardY + BoardHeight + BoardPenWidth + shadowWidth / 2,
                     BoardX + BoardWidth + BoardPenWidth + shadowWidth / 2,
                     BoardY + BoardHeight + BoardPenWidth + shadowWidth / 2);
    painter.setPen(QPen(QColor(30, 15, 15, 255 * showRatio), 8));
    painter.drawLine(BoardX + SquareWidth * 3 + 10,
                     BoardY + 10,
                     BoardX + SquareWidth * 4 - 10,
                     BoardY + SquareWidth - 10);
    painter.drawLine(BoardX + SquareWidth * 3 + 10,
                     BoardY + SquareWidth - 10,
                     BoardX + SquareWidth * 4 - 10,
                     BoardY + 10);
}

GrooveBar::GrooveBar(QWidget *parent)
    : PlayWindowWidget(parent)
{
    _Font = fontLoader("STENCIL.TTF", 35);
}
void GrooveBar::Paint()
{
    bool doFlash = ((ftimer->getCurrentTime() - startflashtime) / 5 + 1) % 2;
    int width = 20;
    int BoardX = (WINDOW_WIDTH - BoardWidth) / 2 - 30 - width;
    drawEdge(BoardX, BoardY, width, BoardHeight);
    QPainter printer(this);
    printer.setPen(QPen(Qt::transparent));
    printer.setBrush(QBrush(QColor(127, 127, 127, 127 * showRatio)));
    for (int i = 0; i < 10 - grooveLevel; i++) {
        printer.drawRect(BoardX + 5, BoardY + 14 + 78 * i, 10, 70);
    }
    for (int i = 10 - grooveLevel; i < 10; i++) {
        if (flashing) {
            if (doFlash)
                printer.setBrush(QBrush(QColor(240, 240, 240)));
            else
                printer.setBrush(QBrush(QColor(240, 0, 0)));

        } else
            printer.setBrush(QBrush(GrooveColor[i]));
        printer.drawRect(BoardX + 5, BoardY + 14 + 78 * i, 10, 70);
    }
    if (grooveLevel) {
        printer.setFont(_Font);
        printer.setPen(QPen(QColor(0, 0, 0, 128)));
        for (int i = -2; i <= 2; i += 2) {
            for (int j = -2; j <= 2; j += 2) {
                printer.drawText(BoardX - 40 + i,
                                 BoardY + 800 - 78 * grooveLevel + j,
                                 100,
                                 58,
                                 Qt::AlignCenter,
                                 RatioString[10 - grooveLevel]);
            }
        }
        printer.setPen(QPen(GrooveColor[10 - grooveLevel]));
        printer.drawText(BoardX - 40,
                         BoardY + 800 - 78 * grooveLevel,
                         100,
                         58,
                         Qt::AlignCenter,
                         RatioString[10 - grooveLevel]);
    }
}
void GrooveBar::startflash()
{
    startflashtime = ftimer->getCurrentTime();
    flashing = true;
}
void GrooveBar::finishflash()
{
    flashing = false;
}

RhythmBar::RhythmBar(QWidget *parent)
    : PlayWindowWidget(parent)
{}
void RhythmBar::Paint()
{
    bool doFlash = ((ftimer->getCurrentTime() - startflashtime) / 5 + 1) % 2;
    int maxLevel = grlvl <= 9 ? grlvl : 9;
    int width = 20;
    int BoardX = (WINDOW_WIDTH + BoardWidth) / 2 + 30;
    drawEdge(BoardX, BoardY, width, BoardHeight);
    if (flashing) {
        if (rhythmLevel <= 10 + maxLevel)
            rhythmLevel = 11 + maxLevel;
    }
    QPainter printer(this);
    printer.setPen(QPen(Qt::transparent));
    printer.setBrush(QBrush(QColor(127, 127, 127, 127 * showRatio)));
    for (int i = 0; i < 20 - rhythmLevel-1; i++) {
        printer.drawRect(BoardX + 5, BoardY + 14 + 39 * i, 10, 31);
    }
    for (int i = 20 - rhythmLevel - 1; i < 20; i++) {
        if (i < 0) {
            continue;
        } else if (i == 20 - rhythmLevel - 1 && !flashing) {
            if (i < 10 - maxLevel) {
                printer.setBrush(QBrush(QColor(120, 0, 0)));
            } else {
                printer.setBrush(QBrush(QColor(0, 0, 120)));
            }
        } else if (flashing) {
            if (doFlash) {
                printer.setBrush(QBrush(QColor(240, 240, 240)));
            } else {
                printer.setBrush(QBrush(QColor(240, 0, 0)));
            }
        }
        else if (i < 10 - maxLevel) {
            printer.setBrush(QBrush(QColor(240, 0, 0)));
        } else {
            printer.setBrush(QBrush(QColor(0, 0, 240)));
        }
        printer.drawRect(BoardX + 5, BoardY + 14 + 39 * i, 10, 31);
    }
}
void RhythmBar::startflash()
{
    startflashtime = ftimer->getCurrentTime();
    flashing = true;
}
void RhythmBar::finishflash()
{
    flashing = false;
}
void RhythmBar::upd(long long crrtm)//realtime
{
    if (stop)
        return;
    if (flashing)
        return;
    int during = (crrtm * bpm * 40 / FPS / 1000) % 40;
    if (during <= 20) {
        rhythmLevel = 20 - during;
    } else {
        rhythmLevel = during - 20;
    }
}
void RhythmBar::setgrlvl(int lvl)
{
    grlvl = lvl;
}
NextBlockBoard::NextBlockBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    _Font = fontLoader("STENCIL.TTF", 40);
}
NextBlockBoard::~NextBlockBoard()
{
    for (int i = 0; i < 4; i++) {
        delete next[i];
    }
}
void NextBlockBoard::Paint()
{
    int width = 280;
    int BoardX = (WINDOW_WIDTH + BoardWidth) / 2 + 80;
    int BoardY0 = BoardY;
    drawEdge(BoardX, BoardY0, width, 280);
    QPainter painter(this);
    painter.setPen(QPen(QColor(240, 240, 120, 255 * showRatio)));
    painter.setFont(_Font);
    painter.drawText(BoardX, BoardY0 + 10, width, 50, Qt::AlignCenter, "NEXT");
    painter.setPen(QPen(QColor(0, 127, 0, 63 * showRatio), 4));
    painter.drawLine(BoardX + BoardPenWidth / 2,
                     BoardY0 + 70,
                     BoardX + width - BoardPenWidth / 2,
                     BoardY0 + 70);
    painter.drawLine(BoardX + 100, BoardY0 + 74, BoardX + 100, BoardY0 + 278 - BoardPenWidth / 2);
    painter.drawLine(BoardX + 190, BoardY0 + 74, BoardX + 190, BoardY0 + 278 - BoardPenWidth / 2);
    for (int i = 0; i < initializeindex; i++) {
        next[i]->move(BoardX + 30 + 90 * i, BoardY0 + 100);
    }
}
column *NextBlockBoard::newColumn()
{
    int clr[3] = {4, 4, 4};
    switch (QRandomGenerator::global()->bounded(10)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6: {
        clr[0] = QRandomGenerator::global()->bounded(4);
        clr[1] = QRandomGenerator::global()->bounded(3);
        if (clr[1] >= clr[0]) {
            clr[1]++;
        }
        clr[2] = QRandomGenerator::global()->bounded(2);
        if (clr[2] >= clr[1]) {
            clr[2]++;
            if (clr[2] >= clr[0])
                clr[2]++;
        } else if (clr[2] >= clr[0]) {
            clr[2]++;
            if (clr[2] >= clr[1])
                clr[2]++;
        }
    } break;
    case 7:
    case 8: {
        clr[0] = clr[1] = clr[2] = QRandomGenerator::global()->bounded(4);
        int num = QRandomGenerator::global()->bounded(3);
        int cl = QRandomGenerator::global()->bounded(3);
        if (cl >= clr[0])
            cl++;
        clr[num] = cl;
    } break;
    case 9:
        clr[0] = clr[1] = clr[2] = QRandomGenerator::global()->bounded(4);
        break;
    }
    return new column(parentWidget(), blockColor(clr[0]), blockColor(clr[1]), blockColor(clr[2]));
}
void NextBlockBoard::initialnew(int cntdwnnum)
{
    if (initialized)
        return;
    else if (cntdwnnum <= initializeindex) {
        next[initializeindex] = newColumn();
        if (initializeindex < 3) {
            next[initializeindex]->show();
        }
        if (initializeindex == 3) {
            initialized = true;
        }
        initializeindex++;
    }
}
column *NextBlockBoard::popColumn()
{
    column *ret = next[0];
    for (int i = 0; i < 3; i++) {
        next[i] = next[i + 1];
    }
    next[2]->show();

    next[3] = newColumn();
    return ret;
}

ScoreBoard::ScoreBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    TitleFont = fontLoader("STENCIL.TTF", 40);
    WordFont = fontLoader("UDDigiKyokashoN-R.ttc", 25);
    DigitFont = fontLoader("Digital-Readout.ttf", 45);
    smallFont = fontLoader("UDDigiKyokashoN-R.ttc", 12);
}
void ScoreBoard::Paint()
{
    int width = 280;
    int BoardX = (WINDOW_WIDTH - BoardWidth) / 2 - 80 - width;
    int BoardY0 = BoardY;
    drawEdge(BoardX, BoardY0, width, 385);
    QPainter painter(this);
    painter.setPen(QPen(QColor(240, 240, 120, 255 * showRatio)));
    painter.setFont(TitleFont);
    painter.drawText(BoardX, BoardY0 + 10, width, 50, Qt::AlignCenter, "SCOREBOARD");
    painter.setFont(WordFont);
    painter.drawText(BoardX, BoardY0 + 55, width, 30, Qt::AlignCenter, "Best Drop");
    painter.drawText(BoardX, BoardY0 + 115, width, 30, Qt::AlignCenter, "Current Drop");
    painter.drawText(BoardX, BoardY0 + 310, width, 30, Qt::AlignCenter, "Total Score");
    painter.setFont(smallFont);
    painter.drawText(BoardX,
                     BoardY0 + 145,
                     width - 145,
                     30,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "Groove Ratio");
    painter.drawText(BoardX,
                     BoardY0 + 145,
                     width - 25,
                     30,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "Chain Ratio");
    painter.drawText(BoardX,
                     BoardY0 + 200,
                     width - 145,
                     30,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "Block Count");
    painter.drawText(BoardX,
                     BoardY0 + 200,
                     width - 25,
                     30,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "Add Score");
    painter.drawText(BoardX,
                     BoardY0 + 255,
                     width - 25,
                     30,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "Drop Score");
    painter.setFont(DigitFont);
    painter.setPen(QPen(QColor(31, 31, 31, 255 * showRatio)));
    painter.drawText(BoardX,
                     BoardY0 + 75,
                     width - 20,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "888888888888");
    painter.drawText(BoardX,
                     BoardY0 + 160,
                     width - 145,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "88.88x");
    painter.drawText(BoardX,
                     BoardY0 + 160,
                     width - 25,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "88.88x");
    painter.drawText(BoardX,
                     BoardY0 + 215,
                     width - 145,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "888=");
    painter.drawText(BoardX,
                     BoardY0 + 215,
                     width - 25,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "88888");
    painter.drawText(BoardX,
                     BoardY0 + 270,
                     width - 20,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "888888888888");
    painter.drawText(BoardX,
                     BoardY0 + 330,
                     width - 20,
                     50,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "888888888888");

    painter.setPen(QPen(QColor(120, 240, 120, 255 * showRatio)));
    QString bstdrp = "";
    bstdrp += std::to_string(bestdrop);
    painter.drawText(BoardX, BoardY0 + 75, width - 20, 50, Qt::AlignRight | Qt::AlignVCenter, bstdrp);
    if (eliminating) {
        painter.setPen(QPen(GrooveColor[10 - groove]));
        painter.drawText(BoardX,
                         BoardY0 + 160,
                         width - 145,
                         50,
                         Qt::AlignRight | Qt::AlignVCenter,
                         RatioString[10 - groove]);
        painter.setPen(QPen(GrooveColor[10 - chain]));
        painter.drawText(BoardX,
                         BoardY0 + 160,
                         width - 25,
                         50,
                         Qt::AlignRight | Qt::AlignVCenter,
                         RatioString[10 - chain]);
        painter.setPen(QPen(QColor(120, 240, 120, 255 * showRatio)));
        QString blknm = "";
        blknm += std::to_string(blocknum);
        blknm += "=";
        painter.drawText(BoardX,
                         BoardY0 + 215,
                         width - 145,
                         50,
                         Qt::AlignRight | Qt::AlignVCenter,
                         blknm);
        QString adscr = "";
        adscr += std::to_string(addscore);
        painter.drawText(BoardX,
                         BoardY0 + 215,
                         width - 25,
                         50,
                         Qt::AlignRight | Qt::AlignVCenter,
                         adscr);
        QString crrntdrp = "";
        crrntdrp += std::to_string(currentdrop);
        painter.drawText(BoardX,
                         BoardY0 + 270,
                         width - 20,
                         50,
                         Qt::AlignRight | Qt::AlignVCenter,
                         crrntdrp);
    }
    QString ttldrp = "";
    ttldrp += std::to_string(totalscore);
    painter
        .drawText(BoardX, BoardY0 + 330, width - 20, 50, Qt::AlignRight | Qt::AlignVCenter, ttldrp);
}
void ScoreBoard::eliminate(int blknm)
{
    eliminating = true;
    blocknum = blknm;
    groove = grooveLevel;
    if (chain < 9) {
        chain++;
    }
    addscore = blocknum * Ratio[10-chain] * Ratio[10-groove];
    currentdrop += addscore;
    if (bestdrop < currentdrop) {
        bestdrop = currentdrop;
    }
    totalscore += addscore;
}
void ScoreBoard::clearCurrent()
{
    eliminating = false;
    chain = 0;
    blocknum = 0;
    addscore = 0;
    currentdrop = 0;
}

BottomRightBoard::BottomRightBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    WordFont = fontLoader("STENCIL.TTF", 20);
    DigitFont = fontLoader("Digital-Readout.ttf", 70);
}
void BottomRightBoard::Paint()
{
    long long tm = (ftimer->getCurrentTime() - frameTimeOffset) * 1000 / FPS;//in ms
    int width = 280;
    int BoardX = (WINDOW_WIDTH + BoardWidth) / 2 + 80;
    int BoardY0 = BoardY + 415;
    drawEdge(BoardX, BoardY0, width, 385);
    QPainter painter(this);
    painter.setFont(WordFont);
    painter.setPen(QPen(QColor(240, 240, 120, 255 * showRatio)));
    painter.drawText(BoardX, BoardY0+10, width, 30, Qt::AlignCenter, "BPM");
    painter.drawText(BoardX, BoardY0+110, width, 40, Qt::AlignCenter, "BEATS");
    painter.drawText(BoardX, BoardY0+260, width, 40, Qt::AlignCenter, "TIME");
    painter.setFont(DigitFont);
    painter.setPen(QPen(QColor(31, 31, 31, 255 * showRatio)));
    painter.drawText(BoardX, BoardY0 + 35, width - 100, 80, Qt::AlignRight | Qt::AlignVCenter, "888");
    painter.drawText(BoardX,
                     BoardY0 + 135,
                     width - 80,
                     80,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "888:8");
    painter.drawText(BoardX,
                     BoardY0 + 285,
                     width - 10,
                     80,
                     Qt::AlignRight | Qt::AlignVCenter,
                     "888:88.888");
    painter.setPen(QPen(QColor(240, 240, 120, 255 * showRatio)));
    QString bpmstr;
    bpmstr += std::to_string(bpm);
    QString beatstr;
    beatstr += std::to_string(bars);
    beatstr += ":";
    beatstr += std::to_string(beats);
    QString timestr;
    if (beats == 0) {
        timestr = "0.000";
    } else {
        if (tm / 60000) {
            timestr += std::to_string(tm / 60000);
            timestr += ":";
            if (tm % 60000 / 10000 == 0) {
                timestr += "0";
            }
        }
        timestr += std::to_string(tm % 60000 / 1000);
        timestr += ".";
        if (tm % 1000 / 100 == 0) {
            timestr += "0";
            if (tm % 100 / 10 == 0) {
                timestr += "0";
            }
        }
        timestr += std::to_string(tm % 1000);
    }
    painter
        .drawText(BoardX, BoardY0 + 35, width - 100, 80, Qt::AlignRight | Qt::AlignVCenter, bpmstr);
    painter.drawText(BoardX,
                     BoardY0 + 135,
                     width - 80,
                     80,
                     Qt::AlignRight | Qt::AlignVCenter,
                     beatstr);
    painter.drawText(BoardX,
                     BoardY0 + 285,
                     width - 10,
                     80,
                     Qt::AlignRight | Qt::AlignVCenter,
                     timestr);
    painter.setPen(Qt::transparent);
    float deltatm = tm * bpm / float(60000) - totalbeats;
    float shine = (cos(qDegreesToRadians(deltatm * 180)) + 1) / 2;
    for (int i = 0; i < beatsPerBar; i++) {
        if (i == beats - 1 && !stop) {
            if (i == 0)
                painter.setBrush(
                    QBrush(QColor(128 * shine + 127, 128 * shine + 127, 127 - 127 * shine)));
            else
                painter.setBrush(
                    QBrush(QColor(127 - 127 * shine, 128 * shine + 127, 128 * shine + 127)));
        } else {
            painter.setBrush(QBrush(QColor(127, 127, 127)));
        }
        painter.drawRect(BoardX + 20 + (250 / beatsPerBar) * i,
                         BoardY0 + 220,
                         250 / beatsPerBar - 10,
                         30);
    }

}
void BottomRightBoard::init(int bpb)
{
    beatsPerBar = bpb;
}
void BottomRightBoard::setbeat(int _b)
{
    if (stop)
        return;
    _b--;
    totalbeats = _b;
    bars = _b / beatsPerBar;
    beats = _b % beatsPerBar + 1;
}

BottomLeftBoard::BottomLeftBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    TitleFont = fontLoader("STENCIL.TTF", 40);
    ContentFont = fontLoader("UDDigiKyokashoN-R.ttc", 20);
}
void BottomLeftBoard::Paint()
{
    int width = 280;
    int BoardY0 = BoardY + 415;
    int BoardX = (WINDOW_WIDTH - BoardWidth) / 2 - 80 - width;
    drawEdge(BoardX, BoardY0, width, 385);
    QPainter painter(this);
    painter.setPen(QPen(QColor(240, 240, 120, 255 * showRatio)));
    painter.setFont(TitleFont);
    painter.drawText(BoardX, BoardY0 + 10, width, 50, Qt::AlignCenter, "MUSIC");
    painter.setFont(ContentFont);
    painter.drawText(BoardX, BoardY0 + 280, width, 100, Qt::AlignCenter, "Name\n"+songname + "\nArtist\n" + songartist);
    QTransform tsfm;
    tsfm.translate(BoardX + 40, BoardY0 + 70);
    tsfm.scale(200.0 / cover.width(), 200.0 / cover.width());
    painter.setTransform(tsfm);
    painter.drawImage(0, 0, cover);
}
void BottomLeftBoard::loadcover(QString name)
{
    cover.load("./res/pics/" + name);
}
void BottomLeftBoard::setsong(QString name, QString artist)
{
    songname = name;
    songartist = artist;
}


CountDown::CountDown(QWidget *parent)
    : PlayWindowWidget(parent)
{
    _Font = fontLoader("STENCIL.TTF", 200);
    showRatio = 1;
}
void CountDown::Paint()
{
    if (cntdwn <= 4 || stop) {
        QString cntdwnstring;
        QPainter painter(this);
        painter.setFont(_Font);
        if (cntdwn <= 4) {
            switch (cntdwn) {
            case 1:
                cntdwnstring = "3";
                break;
            case 2:
                cntdwnstring = "2";
                break;
            case 3:
                cntdwnstring = "1";
                break;
            case 4:
                cntdwnstring = "GO!";
                break;
            default:
                return;
            }
            painter.setPen(QPen(QColor(120, 120, 60, 128)));
            painter.drawText(20, 20, WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, Qt::AlignCenter, cntdwnstring);
            painter.setPen(QPen(QColor(240, 240, 120)));
            painter.drawText(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Qt::AlignCenter, cntdwnstring);
        }
        else if (stop) {
            cntdwnstring = "GAME   OVER";
            painter.setPen(QPen(QColor(60, 60, 60, 128)));
            painter.drawText(20, 20, WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, Qt::AlignCenter, cntdwnstring);
            painter.setPen(QPen(QColor(120, 120, 120)));
            painter.drawText(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Qt::AlignCenter, cntdwnstring);
        }
    }
}
void CountDown::setcntdwn(int cnt)
{
    cntdwn = cnt;
}
