#include "playwindowwidgets.h"
#include "frametimer.h"
#include "predefines.h"
#include "utils.h"

const int SquareWidth = 50;
const int BoardColumns = 8;
const int BoardLines = 16;
const int BoardWidth = BoardColumns * SquareWidth;
const int BoardHeight = BoardLines * SquareWidth;
const int BoardY = 80;
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
QString GrooveRatio[10]
    = {"20.0X", "10.0X", "6.00X", "4.00X", "3.20X", "2.40X", "2.00X", "1.60X", "1.25X", "1.00X"};

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
}

GrooveBar::GrooveBar(QWidget *parent)
    : PlayWindowWidget(parent)
{
    _Font = fontLoader("STENCIL.TTF", 35);
}
void GrooveBar::Paint()
{
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
                                 GrooveRatio[10 - grooveLevel]);
            }
        }
        printer.setPen(QPen(GrooveColor[10 - grooveLevel]));
        printer.drawText(BoardX - 40,
                         BoardY + 800 - 78 * grooveLevel,
                         100,
                         58,
                         Qt::AlignCenter,
                         GrooveRatio[10 - grooveLevel]);
    }
}

RhythmBar::RhythmBar(QWidget *parent)
    : PlayWindowWidget(parent)
{}
void RhythmBar::Paint()
{
    int maxLevel = grooveLevel <= 9 ? grooveLevel : 9;
    int width = 20;
    int BoardX = (WINDOW_WIDTH + BoardWidth) / 2 + 30;
    drawEdge(BoardX, BoardY, width, BoardHeight);
    QPainter printer(this);
    printer.setPen(QPen(Qt::transparent));
    printer.setBrush(QBrush(QColor(127, 127, 127, 127 * showRatio)));
    for (int i = 0; i < 20 - rhythmLevel-1; i++) {
        printer.drawRect(BoardX + 5, BoardY + 14 + 39 * i, 10, 31);
    }
    for (int i = 20 - rhythmLevel - 1; i < 20; i++) {
        if (i < 0) {
            continue;
        } else if (i == 20 - rhythmLevel - 1) {
            if (i < 10 - maxLevel) {
                printer.setBrush(QBrush(QColor(120, 0, 0)));
            } else {
                printer.setBrush(QBrush(QColor(0, 0, 120)));
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
void RhythmBar::flash() {}
void RhythmBar::upd()
{
    int during = ((ftimer->getCurrentTime() - frameTimeOffset) * bpm * 40 / FPS / 60) % 40;
    if (during <= 20) {
        rhythmLevel = 20 - during;
    } else {
        rhythmLevel = during - 20;
    }
}

NextBlockBoard::NextBlockBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    _Font = fontLoader("STENCIL.TTF", 40);
}
void NextBlockBoard::Paint()
{
    int width = 280;
    int BoardX = (WINDOW_WIDTH + BoardWidth) / 2 + 80;
    int BoardY0 = BoardY;
    drawEdge(BoardX, BoardY0, width, 280);

}

ScoreBoard::ScoreBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    WordFont = fontLoader("STENCIL.TTF", 40);
    DigitFont = fontLoader("Digital-Readout.ttf", 40);
}
void ScoreBoard::Paint()
{
    int width = 280;
    int BoardX = (WINDOW_WIDTH - BoardWidth) / 2 - 80 - width;
    int BoardY0 = BoardY;
    drawEdge(BoardX, BoardY0, width, 385);
}

MusicBoard::MusicBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    WordFont = fontLoader("STENCIL.TTF", 40);
    DigitFont = fontLoader("Digital-Readout.ttf", 40);
}
void MusicBoard::Paint()
{
    int width = 280;
    int BoardX = (WINDOW_WIDTH + BoardWidth) / 2 + 80;
    int BoardY0 = BoardY + 415;
    drawEdge(BoardX, BoardY0, width, 385);
}

TimeBoard::TimeBoard(QWidget *parent)
    : PlayWindowWidget(parent)
{
    WordFont = fontLoader("STENCIL.TTF", 40);
    DigitFont = fontLoader("Digital-Readout.ttf", 40);
}
void TimeBoard::Paint()
{
    int width = 280;
    int BoardY0 = BoardY + 415;
    int BoardX = (WINDOW_WIDTH - BoardWidth) / 2 - 80 - width;
    drawEdge(BoardX, BoardY0, width, 385);
}

CountDown::CountDown(QWidget *parent)
    : PlayWindowWidget(parent)
{
    _Font = fontLoader("STENCIL.TTF", 200);
    showRatio = 1;
}
void CountDown::Paint()
{
    if (cntdwn > 4)
        return;
    QString cntdwnstring;
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
    QPainter painter(this);
    painter.setFont(_Font);
    painter.setPen(QPen(QColor(120, 120, 60, 128)));
    painter.drawText(20, 20, WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, Qt::AlignCenter, cntdwnstring);
    painter.setPen(QPen(QColor(240, 240, 120)));
    painter.drawText(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Qt::AlignCenter, cntdwnstring);

}
void CountDown::setcntdwn(int cnt)
{
    cntdwn = cnt;
}
