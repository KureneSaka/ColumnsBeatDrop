#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <Qdebug>
#include <QPainter>

#include "predefines.h"
#include "frametimer.h"

extern FrameTimer *ftimer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    ui->setupUi(this);
    setWindowTitle("Columns Beat Drop");
    bg = new BackGround(this);
    bg->show();
    pw = new PlayWindow(this);
    pw->show();
    mm = new MainMenu(this);
    mm->show();
    connect(ftimer, &FrameTimer::frameRefresh, this, [=]() { update(); });
    connect(mm, &MainMenu::ExitGame, this, [=]() { this->close(); });
    connect(mm, &MainMenu::StartGame, this, [=]() {
        pm = new PlayMenu(this);
        pm->show();
        connect(pm, &PlayMenu::GameStart, this, [=]() { pw->Initialize(); });
    });
    mm->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete bg;
    if (mm)
        delete mm;
    if (pm)
        delete pm;
}

