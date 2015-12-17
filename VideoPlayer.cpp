#include "VideoPlayer.h"

#include <QDateTime>
#include <QFileInfo>

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

    ui.comboSpeed->addItem("0.25x", QVariant(0.25));
    ui.comboSpeed->addItem("0.5x", QVariant(0.5));
    ui.comboSpeed->addItem("1.0x", QVariant(1.0));
    ui.comboSpeed->addItem("2.0x", QVariant(2.0));
    ui.comboSpeed->addItem("5.0x", QVariant(5.0));
    ui.comboSpeed->setCurrentIndex(2);

    connect(ui.btStop, SIGNAL(clicked()), _player, SLOT(stop()));
    connect(ui.btPlay, SIGNAL(clicked()), SLOT(onPlay()));
    connect(ui.btPrev, SIGNAL(clicked()), SLOT(onPrev()));
    connect(ui.btNext, SIGNAL(clicked()), SLOT(onNext()));
    connect(ui.btMute, SIGNAL(clicked()), SLOT(onMute()));

    connect(ui.sliderVolume,    SIGNAL(sliderMoved(int)), _player, SLOT(setVolume(int)));
    connect(ui.comboSpeed,      SIGNAL(activated(int)),     SLOT(onSpeedChanged()));
    connect(ui.sliderProgress,  SIGNAL(sliderMoved(int)),   SLOT(onSeek(int)));

    connect(_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            SLOT(setState(QMediaPlayer::State)));
    connect(_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(_player, SIGNAL(durationChanged(qint64)), SLOT(onDurationChanged(qint64)));
    connect(_player, SIGNAL(positionChanged(qint64)), SLOT(onPositionChanged(qint64)));

    _muted = false;
    _toSeek = -1;
}

/**
 * Play a given file
 */
void VideoPlayer::play(const QString& filePath)
{
    if (filePath.isEmpty())
        return;

    _filePath = filePath;
    _player->setMedia(QUrl::fromLocalFile(filePath));
    _player->play();
    setWindowTitle("Video Player - " + QFileInfo(filePath).fileName());
}

/**
 * Play a file at the given position
 */
void VideoPlayer::play(const QString& filePath, int seconds)
{
    play(filePath);
    _toSeek = seconds;  // mark the position and wait the file to be buffered
}

/**
 * Seek to the pre-set position when the file is buffered
 */
void VideoPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::BufferedMedia && _toSeek > -1)
        onSeek(_toSeek);
}

void VideoPlayer::pause() {
    _player->pause();
}

void VideoPlayer::onPlay()
{
    if (_state == QMediaPlayer::PlayingState)
        pause();
    else
        _player->play();
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
        previousVolume = ui.sliderVolume->value();  // backup volume
        ui.sliderVolume->setValue(0);
    }
    else
        ui.sliderVolume->setValue(previousVolume);  // restore previous volume
}

void VideoPlayer::onSpeedChanged()
{
    double speed = ui.comboSpeed->itemData(ui.comboSpeed->currentIndex()).toDouble();
    _player->setPlaybackRate(speed);
}

/**
 * When video length changed
 * @param duration  - video length in msec
 */
void VideoPlayer::onDurationChanged(qint64 duration) {
    ui.sliderProgress->setMaximum(duration / 1000);
}

/**
 * When video play position is changed
 * @param position  - in msec
 */
void VideoPlayer::onPositionChanged(qint64 position)
{
    const qint64 seconds = position / 1000;
    ui.sliderProgress->setValue(seconds);

    // update progress label
    QString output;
    const qint64 totalSeconds = ui.sliderProgress->maximum();
    QTime currentTime((seconds/3600)%60, (seconds/60)%60, seconds%60, (seconds*1000)%1000);
    QTime totalTime((totalSeconds/3600)%60, (totalSeconds/60)%60, totalSeconds%60, (totalSeconds*1000)%1000);
    QString format = seconds > 3600 ? "mm:ss" : "hh:mm:ss";
    output = currentTime.toString(format) + " / " + totalTime.toString(format);
    ui.labelProgress->setText(output);
}

/**
 * Seek the file to the given position
 * Progress slider will be updated via onPositionChanged
 */
void VideoPlayer::onSeek(int seconds)
{
    _player->setPosition(seconds * 1000);
    _toSeek = -1;
}

/**
 * Update UI states based on player state
 */
void VideoPlayer::setState(QMediaPlayer::State state)
{
    _state = state;

    switch (state)
    {
    case QMediaPlayer::StoppedState:
        ui.btStop->setEnabled(false);
        ui.btPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        onSeek(0);
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

