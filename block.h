#ifndef BLOCK_H
#define BLOCK_H
#include <QWidget>

enum blockColor { Bred, Bgreen, Bblue, Byellow, BNULL };

class block : public QWidget
{
    Q_OBJECT
    blockColor blkclr;
    bool toEliminate = false;
    int eliminateGroup = 0;
public:
    block(QWidget *parent = nullptr, blockColor clr = BNULL);
    blockColor getBlockColor();
    void setEliminate(bool b);
    void setEliminateGroup(int i);
    bool getEliminate();
    int getEliminateGroup();
private:
    void paintEvent(QPaintEvent *);
};

class column : public QWidget
{
    Q_OBJECT
    block blocks[3];
    float edgeshine = 0;
    int exchangeind = 0;

public:
    column(QWidget *parent = nullptr,
           blockColor clr1 = BNULL,
           blockColor clr2 = BNULL,
           blockColor clr3 = BNULL);
    void setshine(float sh);
    blockColor getBlockColor(int index);
    void exchange(bool is_up);
private:
    void paintEvent(QPaintEvent *);
};

#endif // BLOCK_H
