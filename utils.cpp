#include "utils.h"
#include <QFontDatabase>
#include <QCoreApplication>

QFont fontLoader(QString name, int fontSize)
{
    int fontId = QFontDatabase::addApplicationFont("./res/fonts/" + name);
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont retFont = QFont(fontName);
    retFont.setPixelSize(fontSize);
    return retFont;
}
