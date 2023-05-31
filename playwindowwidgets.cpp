#include "playwindowwidgets.h"

const int SquareWidth=50;
const int BoardColumns=8;
const int BoardLines=16;
const int BoardX=120;
const int BoardY=80;
const int BoardWidth=BoardColumns*SquareWidth;
const int BoardHeight=BoardLines*SquareWidth;

PlayWindowWidget::PlayWindowWidget(QWidget *parent):QWidget(parent)
{
    move(0,0);
    resize(1280,960);
}
void PlayWindowWidget::setRatio(float shapeRatio)
{
    showRatio=shapeRatio;
}

PlayBoard::PlayBoard(QWidget *parent):PlayWindowWidget(parent){}
void PlayBoard::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::transparent));
    painter.setBrush(QBrush(QColor(0,0,0,240*showRatio)));
    painter.drawRect(BoardX,BoardY,BoardWidth,BoardHeight);
    painter.setPen(QPen(QColor(0,127,0,63*showRatio),1));
    for(int i=0;i<BoardLines;i++)
    {
        painter.drawLine(BoardX,BoardY+i*SquareWidth,BoardX+BoardWidth,BoardY+i*SquareWidth);
    }
    for(int i=1;i<=BoardColumns-1;i++)
    {
        painter.drawLine(BoardX+i*SquareWidth,BoardY,BoardX+i*SquareWidth,BoardY+BoardHeight);
    }
    painter.setPen(QPen(QColor(0,200,0,255*showRatio),4));
    painter.drawLine(BoardX-2,BoardY,BoardX-2,BoardY+BoardHeight+2);
    painter.drawLine(BoardX+BoardWidth+2,BoardY,BoardX+BoardWidth+2,BoardY+BoardHeight+2);
    painter.drawLine(BoardX-2,BoardY+BoardHeight+2,BoardX+BoardWidth+2,BoardY+BoardHeight+2);
    painter.setPen(QPen(QColor(0,127,0,128*showRatio),10));
    painter.drawLine(BoardX+BoardWidth+9,BoardY+13,BoardX+BoardWidth+9,BoardY+BoardHeight-1);
    painter.drawLine(BoardX+11,BoardY+BoardHeight+9,BoardX+BoardWidth+9,BoardY+BoardHeight+9);
}

GrooveBar::GrooveBar(QWidget *parent):PlayWindowWidget(parent){}
void GrooveBar::paintEvent(QPaintEvent*)
{

}
