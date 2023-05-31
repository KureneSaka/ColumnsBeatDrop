#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>


class MusicPlayer:public QMediaPlayer
{
    Q_OBJECT
    QAudioOutput *otpt;
public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    void readMusic(QString name);
    void readSound(QString name);
    void cleanplay();
};

#endif // MUSICPLAYER_H
