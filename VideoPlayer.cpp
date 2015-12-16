#include "VideoPlayer.h"

#include <QMediaPlaylist>

VideoPlayer::VideoPlayer(QWidget* parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    ui.btPlay->setIcon((style()->standardIcon(QStyle::SP_MediaPlay)));
    ui.btStop->setIcon((style()->standardIcon(QStyle::SP_MediaStop)));
    ui.btPrev->setIcon((style()->standardIcon(QStyle::SP_MediaSkipBackward)));
    ui.btNext->setIcon((style()->standardIcon(QStyle::SP_MediaSkipForward)));
    ui.btMute->setIcon((style()->standardIcon(QStyle::SP_MediaVolume)));

    _player = new QMediaPlayer;
    _player->setVideoOutput(ui.videoWidget);

    _playList = new QMediaPlaylist;

    connect(ui.btPlay, SIGNAL(clicked()), this, SLOT(onPlay()));
    connect(ui.btStop, SIGNAL(clicked()), this, SLOT(onStop()));
    connect(ui.btPrev, SIGNAL(clicked()), this, SLOT(onPrev()));
    connect(ui.btNext, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(ui.btMute, SIGNAL(clicked()), this, SLOT(onMute()));

    _muted = false;
}

void VideoPlayer::play(const QString& filePath)
{
    if (filePath.isEmpty())
        return;

    _filePath = filePath;
    _player->setMedia(QUrl::fromLocalFile(filePath));
    _player->play();
    setState(QMediaPlayer::PlayingState);
}

void VideoPlayer::pause()
{
    _player->pause();
    setState(QMediaPlayer::PausedState);
}

void VideoPlayer::addFile(const QString& filePath) {
    _playList->addMedia(QUrl::fromLocalFile(filePath));
}

void VideoPlayer::onPlay()
{
    if (_state == QMediaPlayer::PlayingState)
        pause();
    else
        play(_filePath);
}

void VideoPlayer::onStop()
{
    _player->stop();
    setState(QMediaPlayer::StoppedState);
}

void VideoPlayer::onPrev()
{

}

void VideoPlayer::onNext()
{

}

void VideoPlayer::onMute()
{
    _muted = !_muted;
    ui.btMute->setIcon(style()->standardIcon(_muted
                    ? QStyle::SP_MediaVolumeMuted
                    : QStyle::SP_MediaVolume));

    static int previousVolume = 0;
    if (_muted)
    {
        previousVolume = ui.sliderVolume->value();
        ui.sliderVolume->setValue(0);
    }
    else
        ui.sliderVolume->setValue(previousVolume);
}

void VideoPlayer::onProgress(int)
{

}

void VideoPlayer::setState(QMediaPlayer::State state)
{
        _state = state;

        switch (state)
        {
        case QMediaPlayer::StoppedState:
            ui.btStop->setEnabled(false);
            ui.btPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            ui.btStop->setEnabled(true);
            ui.btPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            ui.btStop->setEnabled(true);
            ui.btPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
}
