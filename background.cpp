#include "background.h"
#include "predefines.h"
#include "frametimer.h"

extern FrameTimer *ftimer;
extern int bpm;
bool music_on = false;
extern long long frameTimeOffset;
BackGround::BackGround(QWidget *parent)
    : QWidget(parent)
{
    move(0, 0);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void BackGround::paintEvent(QPaintEvent *)
{
    long long frametime = ftimer->getCurrentTime();

    QPainter painter(this);
    painter.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Qt::black);
    painter.drawPoint(MIDDLE_WIDTH, MIDDLE_HEIGHT);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QTransform tPosition;
    tPosition.translate(MIDDLE_WIDTH, MIDDLE_HEIGHT);

    QTransform tAngleUnitCW;
    tAngleUnitCW.rotate(6);
    QTransform tAngleUnitRCW;
    tAngleUnitRCW.rotate(-12);

    QColor BaseColor = QColor(240, 0, 0);

    QTransform tOuterAngle;
    tOuterAngle.rotate(double(frametime) / 20);
    QTransform tInnerAngle;
    tInnerAngle.rotate(-double(frametime) / 20);

    for (int i = 0; i < 60; i++) {
        if (i < 20) {
            BaseColor.setRed(BaseColor.red() - 12);
            BaseColor.setGreen(BaseColor.green() + 12);
        } else if (i >= 20 && i < 40) {
            BaseColor.setGreen(BaseColor.green() - 12);
            BaseColor.setBlue(BaseColor.blue() + 12);
        } else if (i >= 40) {
            BaseColor.setBlue(BaseColor.blue() - 12);
            BaseColor.setRed(BaseColor.red() + 12);
        }
        QColor PenColor = BaseColor;
        if (i == 0) {
            PenColor = QColor(240, 240, 240);
        }
        QColor BrushColor = Qt::black;
        if (music_on) {
            int during = ((frametime - frameTimeOffset) * bpm / FPS) % 60;
            if (i == during) {
                BrushColor = PenColor;
            } else if ((i + 60 - during) % 60 == 1 || (during + 60 - i) % 60 == 1) {
                BrushColor.setRgb(PenColor.red() * 0.75,
                                  PenColor.green() * 0.75,
                                  PenColor.blue() * 0.75);
            } else if ((i + 60 - during) % 60 == 2 || (during + 60 - i) % 60 == 2) {
                BrushColor.setRgb(PenColor.red() * 0.5,
                                  PenColor.green() * 0.5,
                                  PenColor.blue() * 0.5);
            } else if ((i + 60 - during) % 60 == 3 || (during + 60 - i) % 60 == 3) {
                BrushColor.setRgb(PenColor.red() * 0.25,
                                  PenColor.green() * 0.25,
                                  PenColor.blue() * 0.25);
            }
        }
        painter.setPen(QPen(PenColor, 2));
        painter.setBrush(QBrush(BrushColor));

        QTransform trans = tOuterAngle * tPosition;
        painter.setTransform(trans);
        painter.drawRect(300, -5, 150, 10);
        tOuterAngle *= tAngleUnitCW;
        if (i % 2 == 0) {
            trans = tInnerAngle * tPosition;
            painter.setTransform(trans);
            painter.drawRect(100, -5, 150, 10);
            tInnerAngle *= tAngleUnitRCW;
        }
    }
}
