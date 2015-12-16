#include "VideoPlayer.h"

#include <QDateTime>
#include <QFileInfo>
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

    _muted = false;

    ui.comboSpeed->addItem("0.25x", QVariant(0.25));
    ui.comboSpeed->addItem("0.5x", QVariant(0.5));
    ui.comboSpeed->addItem("1.0x", QVariant(1.0));
    ui.comboSpeed->addItem("2.0x", QVariant(2.0));
    ui.comboSpeed->addItem("5.0x", QVariant(5.0));
    ui.comboSpeed->setCurrentIndex(2);

    connect(ui.btPlay, SIGNAL(clicked()), this, SLOT(onPlay()));
    connect(ui.btStop, SIGNAL(clicked()), this, SLOT(onStop()));
    connect(ui.btPrev, SIGNAL(clicked()), this, SLOT(onPrev()));
    connect(ui.btNext, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(ui.btMute, SIGNAL(clicked()), this, SLOT(onMute()));

    connect(ui.sliderVolume, SIGNAL(sliderMoved(int)), _player, SLOT(setVolume(int)));
    connect(ui.comboSpeed,   SIGNAL(activated(int)), SLOT(onSpeedChanged()));
    connect(ui.sliderProgress, SIGNAL(sliderMoved(int)), this, SLOT(onSeek(int)));

    connect(_player, SIGNAL(durationChanged(qint64)), SLOT(onDurationChanged(qint64)));
    connect(_player, SIGNAL(positionChanged(qint64)), SLOT(onPositionChanged(qint64)));
}

void VideoPlayer::play(const QString& filePath)
{
    if (filePath.isEmpty())
        return;

    _filePath = filePath;
    _player->setMedia(QUrl::fromLocalFile(filePath));
    _player->play();
    setState(QMediaPlayer::PlayingState);
    setWindowTitle("Video Player - " + QFileInfo(filePath).fileName());
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
    _player->setMuted(_muted);
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

void VideoPlayer::onSpeedChanged() {
    _player->setPlaybackRate(ui.comboSpeed->itemData(ui.comboSpeed->currentIndex()).toDouble());
}

void VideoPlayer::onDurationChanged(qint64 duration) {
    ui.sliderProgress->setMaximum(duration / 1000);
}

void VideoPlayer::onPositionChanged(qint64 position)
{
    const qint64 seconds = position / 1000;
    ui.sliderProgress->setValue(seconds);
    QString output;
    if (seconds > 0)
    {
        const qint64 totalSeconds = ui.sliderProgress->maximum();
        QTime currentTime((seconds/3600)%60, (seconds/60)%60, seconds%60, (seconds*1000)%1000);
        QTime totalTime((totalSeconds/3600)%60, (totalSeconds/60)%60, totalSeconds%60, (totalSeconds*1000)%1000);
        QString format = "mm:ss";
        if (seconds > 3600)
            format = "hh:mm:ss";
        output = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui.labelProgress->setText(output);
}

void VideoPlayer::onSeek(int seconds) {
    _player->setPosition(seconds * 1000);
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
