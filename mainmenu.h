#ifndef MAINMENU_H
#define MAINMENU_H
#include "musicplayer.h"
#include "predefines.h"
#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>

namespace C_MainMenu
{
enum Cursor
{
    Play,
    Setting,
    Help,
    Quit
};
}
class MainMenu : public QMainWindow
{
    Q_OBJECT;
    C_MainMenu::Cursor cursor = C_MainMenu::Play;
    QFont MenuFont;
    QFont TitleFont;
    long long frameTimeOffset = 0;
    State_w stt = S_Normal;
    SoundPlayer select;
    SoundPlayer confirm;
public:
    MainMenu(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void shapeChange(State_w _stt);
signals:
    void StartGame();
    void ExitGame();
public slots:
};

#endif // MAINMENU_H
