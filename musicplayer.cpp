#include "musicplayer.h"
#include <QAudioDevice>
#include <QMediaDevices>

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMediaPlayer(parent)
{
    otpt = new QAudioOutput;
    otpt->setVolume(0.3);
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

SoundPlayer::SoundPlayer(QWidget *parent)
    : QSoundEffect(parent)
{
    setAudioDevice(QMediaDevices::defaultAudioOutput());
}
void SoundPlayer::readSound(QString name)
{
    setSource(QUrl::fromLocalFile("./res/sounds/" + name));
}
