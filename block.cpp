#include "block.h"
#include <QPainter>

const int SquareWidth = 50;
int squaremid = SquareWidth / 2;
QPolygon plgn[8] //0 1 red, 2 3 green, 4 blue, 5 yellow
    {QList<QPoint>{QPoint(squaremid - 20, squaremid),
                   QPoint(squaremid - 10, squaremid - 17.32),
                   QPoint(squaremid + 10, squaremid - 17.32),
                   QPoint(squaremid + 20, squaremid),
                   QPoint(squaremid + 10, squaremid + 17.32),
                   QPoint(squaremid - 10, squaremid + 17.32)},
     QList<QPoint>{QPoint(squaremid - 10, squaremid),
                   QPoint(squaremid - 5, squaremid - 8.66),
                   QPoint(squaremid + 5, squaremid - 8.66),
                   QPoint(squaremid + 10, squaremid),
                   QPoint(squaremid + 5, squaremid + 8.66),
                   QPoint(squaremid - 5, squaremid + 8.66)},
     QList<QPoint>{QPoint(squaremid, squaremid - 20),
                   QPoint(squaremid + 17.32, squaremid + 10),
                   QPoint(squaremid - 17.32, squaremid + 10)},
     QList<QPoint>{QPoint(squaremid, squaremid + 20),
                   QPoint(squaremid + 17.32, squaremid - 10),
                   QPoint(squaremid - 17.32, squaremid - 10)},
     QList<QPoint>{QPoint(squaremid - 20, squaremid - 20),
                   QPoint(squaremid, squaremid - 10),
                   QPoint(squaremid + 20, squaremid - 20),
                   QPoint(squaremid + 10, squaremid),
                   QPoint(squaremid + 20, squaremid + 20),
                   QPoint(squaremid, squaremid + 10),
                   QPoint(squaremid - 20, squaremid + 20),
                   QPoint(squaremid - 10, squaremid)},
     QList<QPoint>{QPoint(squaremid - 5, squaremid),
                   QPoint(squaremid, squaremid - 5),
                   QPoint(squaremid + 5, squaremid),
                   QPoint(squaremid, squaremid + 5)},
     QList<QPoint>{QPoint(squaremid - 5, squaremid - 20),
                   QPoint(squaremid + 5, squaremid - 20),
                   QPoint(squaremid + 5, squaremid + 20),
                   QPoint(squaremid - 5, squaremid + 20)},
     QList<QPoint>{QPoint(squaremid + 20, squaremid + 5),
                   QPoint(squaremid + 20, squaremid - 5),
                   QPoint(squaremid - 20, squaremid - 5),
                   QPoint(squaremid - 20, squaremid + 5)}};

block::block(QWidget *parent, blockColor clr)
    : QWidget(parent)
    , blkclr(clr)
{
    resize(SquareWidth, SquareWidth);
    show();
}
void block::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    switch (blkclr) {
    case Bred:
        painter.setPen(QPen(QColor(240, 60, 60), 3));
        painter.drawPolygon(plgn[0]);
        painter.drawPolygon(plgn[1]);
        break;
    case Bgreen:
        painter.setPen(QPen(QColor(60, 240, 60), 3));
        painter.drawPolygon(plgn[2]);
        painter.drawPolygon(plgn[3]);
        break;
    case Bblue:
        painter.setPen(QPen(QColor(60, 60, 240), 3));
        painter.drawPolygon(plgn[4]);
        painter.drawPolygon(plgn[5]);
        break;
    case Byellow:
        painter.setPen(QPen(QColor(240, 240, 60), 3));
        painter.drawPolygon(plgn[6]);
        painter.drawPolygon(plgn[7]);
        break;
    case BNULL:
        painter.setPen(QPen(QColor(255, 255, 255), 3));
        painter.drawLine(0, 0, SquareWidth, SquareWidth);
        painter.drawLine(0, SquareWidth, SquareWidth, 0);
        break;
    }
}
blockColor block::getBlockColor()
{
    return blkclr;
}
void block::setEliminate(bool b)
{
    toEliminate = b;
}
bool block::getEliminate()
{
    return toEliminate;
}
void block::setEliminateGroup(int gr)
{
    eliminateGroup = gr;
}
int block::getEliminateGroup()
{
    return eliminateGroup;
}


column::column(QWidget *parent, blockColor clr1, blockColor clr2, blockColor clr3)
    : QWidget(parent)
    , blocks{block(this, clr1), block(this, clr2), block(this, clr3)}
{
    resize(SquareWidth, 3 * SquareWidth);
    for (int i = 0; i < 3; i++) {
        blocks[i].move(0, SquareWidth * i);
    }
}
void column::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QColor(200, 200, 200, 255 * edgeshine), 2));
    painter.drawRect(1, 1, SquareWidth - 2, 3 * SquareWidth - 2);
}
blockColor column::getBlockColor(int index)
{
    return blocks[(index + 3 - exchangeind) % 3].getBlockColor();
}
void column::setshine(float sh)
{
    edgeshine = sh;
}
void column::exchange(bool is_up)
{
    if (is_up)
        exchangeind = (exchangeind + 2) % 3;
    else
        exchangeind = (exchangeind + 1) % 3;
    for (int i = 0; i < 3; i++) {
        blocks[i].move(0, SquareWidth * ((i + exchangeind) % 3));
    }

}
