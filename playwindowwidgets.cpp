#include "playwindowwidgets.h"
#include "predefines.h"
#include "utils.h"

const int SquareWidth=50;
const int BoardColumns=8;
const int BoardLines=16;
const int BoardWidth=BoardColumns*SquareWidth;
const int BoardHeight=BoardLines*SquareWidth;
const int BoardY=80;
const int BoardPenWidth=4;

PlayWindowWidget::PlayWindowWidget(QWidget *parent):QWidget(parent)
{
    move(0,0);
    resize(1280,960);
    show();
}
void PlayWindowWidget::setRatio(float shapeRatio)
{
    showRatio=shapeRatio;
}
void PlayWindowWidget::paintEvent(QPaintEvent*)
{
    if(showRatio<=0.01)
        return;
    Paint();
}
void PlayWindowWidget::drawEdge(int x,int y,int w,int h,int shadowWidth)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(QColor(0,200,0,255*showRatio),BoardPenWidth));
    painter.setBrush(QBrush(QColor(0,0,0,200*showRatio)));
    painter.drawRect(x-BoardPenWidth/2,
                     y-BoardPenWidth/2,
                     w+BoardPenWidth,
                     h+BoardPenWidth);
    painter.setPen(QPen(QColor(0,127,0,128*showRatio),shadowWidth));
    QPoint pru(x+w+BoardPenWidth+shadowWidth/2,y-BoardPenWidth/2+shadowWidth*3/2);
    QPoint prd1(x+w+BoardPenWidth+shadowWidth/2,y+h+BoardPenWidth-shadowWidth/2);
    QPoint prd2(x+w+BoardPenWidth+shadowWidth/2,y+h+BoardPenWidth+shadowWidth/2);
    QPoint pld(x-BoardPenWidth/2+shadowWidth*3/2,y+h+BoardPenWidth+shadowWidth/2);
    painter.drawLine(pru,prd1);
    painter.drawLine(pld,prd2);

}

PlayBoard::PlayBoard(QWidget *parent):PlayWindowWidget(parent){}
void PlayBoard::Paint()
{
    int BoardX=(WINDOW_WIDTH-BoardWidth)/2;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::transparent));
    painter.setBrush(QBrush(QColor(0,0,0,240*showRatio)));
    painter.drawRect(BoardX,BoardY,BoardWidth,BoardHeight);
    painter.setPen(QPen(QColor(0,127,0,63*showRatio),1));
    for(int i=1;i<BoardLines;i++)
    {
        painter.drawLine(BoardX,BoardY+i*SquareWidth,BoardX+BoardWidth,BoardY+i*SquareWidth);
    }
    for(int i=1;i<BoardColumns;i++)
    {
        painter.drawLine(BoardX+i*SquareWidth,BoardY,BoardX+i*SquareWidth,BoardY+BoardHeight);
    }
    painter.setPen(QPen(QColor(0,200,0,255*showRatio),BoardPenWidth));
    painter.setBrush(QBrush(Qt::transparent));
    painter.drawRect(BoardX-BoardPenWidth/2,
                     BoardY-BoardPenWidth/2,
                     BoardWidth+BoardPenWidth,
                     BoardHeight+BoardPenWidth);
    int shadowWidth=8;
    painter.setPen(QPen(QColor(0,127,0,128*showRatio),shadowWidth));
    painter.drawLine(BoardX+BoardWidth+BoardPenWidth+shadowWidth/2,
                     BoardY-BoardPenWidth/2+shadowWidth*3/2,
                     BoardX+BoardWidth+BoardPenWidth+shadowWidth/2,
                     BoardY+BoardHeight+BoardPenWidth-shadowWidth/2);
    painter.drawLine(BoardX-BoardPenWidth/2+shadowWidth*3/2,
                     BoardY+BoardHeight+BoardPenWidth+shadowWidth/2,
                     BoardX+BoardWidth+BoardPenWidth+shadowWidth/2,
                     BoardY+BoardHeight+BoardPenWidth+shadowWidth/2);
}

GrooveBar::GrooveBar(QWidget *parent):PlayWindowWidget(parent)
{
    _Font=fontLoader("RationalInteger.ttf",40);
}
void GrooveBar::Paint()
{
    int width=20;
    int BoardX=(WINDOW_WIDTH-BoardWidth)/2-30-width;
    drawEdge(BoardX,BoardY,width,BoardHeight);
}

RythmBar::RythmBar(QWidget *parent):PlayWindowWidget(parent){}
void RythmBar::Paint()
{
    int width=20;
    int BoardX=(WINDOW_WIDTH+BoardWidth)/2+30;
    drawEdge(BoardX,BoardY,width,BoardHeight);
}

NextBlockBoard::NextBlockBoard(QWidget *parent):PlayWindowWidget(parent)
{
    _Font=fontLoader("STENCIL.TTF",40);
}
void NextBlockBoard::Paint()
{
    int width=280;
    int BoardX=(WINDOW_WIDTH+BoardWidth)/2+80;
    int BoardY0=BoardY;
    drawEdge(BoardX,BoardY0,width,280);
}

ScoreBoard::ScoreBoard(QWidget *parent):PlayWindowWidget(parent)
{
    WordFont=fontLoader("STENCIL.TTF",40);
    DigitFont=fontLoader("Digital-Readout.ttf",40);
}
void ScoreBoard::Paint()
{
    int width=280;
    int BoardX=(WINDOW_WIDTH-BoardWidth)/2-80-width;
    int BoardY0=BoardY;
    drawEdge(BoardX,BoardY0,width,385);
}

MusicBoard::MusicBoard(QWidget *parent):PlayWindowWidget(parent)
{
    WordFont=fontLoader("STENCIL.TTF",40);
    DigitFont=fontLoader("Digital-Readout.ttf",40);
}
void MusicBoard::Paint()
{
    int width=280;
    int BoardX=(WINDOW_WIDTH+BoardWidth)/2+80;
    int BoardY0=BoardY+415;
    drawEdge(BoardX,BoardY0,width,385);
}

TimeBoard::TimeBoard(QWidget *parent):PlayWindowWidget(parent)
{
    WordFont=fontLoader("STENCIL.TTF",40);
    DigitFont=fontLoader("Digital-Readout.ttf",40);
}
void TimeBoard::Paint()
{
    int width=280;
    int BoardY0=BoardY+415;
    int BoardX=(WINDOW_WIDTH-BoardWidth)/2-80-width;
    drawEdge(BoardX,BoardY0,width,385);
}
