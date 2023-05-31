#include "musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMediaPlayer(parent)
{
    otpt = new QAudioOutput;
    setAudioOutput(otpt);
}
MusicPlayer::~MusicPlayer()
{
    delete otpt;
}
void MusicPlayer::readMusic(QString name)
{
    setSource(QUrl::fromLocalFile("./res/musics/" + name));
}
void MusicPlayer::readSound(QString name)
{
    setSource(QUrl::fromLocalFile("./res/sounds/" + name));
}
void MusicPlayer::cleanplay()
{
    QMediaPlayer::stop();
    QMediaPlayer::play();
}
