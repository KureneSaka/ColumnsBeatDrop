#include "mainwindow.h"
#include "predefines.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QScreen>
#include "frametimer.h"

FrameTimer *ftimer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Test_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QScreen *currentScreen = QGuiApplication::primaryScreen();
    int x = (currentScreen->size().width() - WINDOW_WIDTH)/2;
    int y = (currentScreen->size().height() - WINDOW_HEIGHT)/2 - 40;
    x = x > 0 ? x : 0;
    y = y > 0 ? y : 0;
    FrameTimer frametimer;
    ftimer = &frametimer;
    MainWindow w;
    w.move(x,y);
    w.show();
    a.exec();
    return 0;
}
