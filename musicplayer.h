#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

class MusicPlayer : public QMediaPlayer
{
    Q_OBJECT
    QAudioOutput *otpt;

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    void readMusic(QString name);
};
class SoundPlayer : public QSoundEffect
{
    Q_OBJECT
    QAudioOutput *otpt;

public:
    SoundPlayer(QWidget *parent = nullptr);
    void readSound(QString name);
};

#endif // MUSICPLAYER_H
