#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "background.h"
#include "mainmenu.h"
#include "playmenu.h"
#include "playwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    BackGround *bg = NULL;
    MainMenu *mm = NULL;
    PlayMenu *pm = NULL;
    PlayWindow *pw = NULL;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
