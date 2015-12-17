#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "ui_VideoPlayer.h"
#include <QMediaPlayer>

/**
 * A video player
 */
class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget* parent = 0);
    void play(const QString& filePath);
    void play(const QString& filePath, int seconds);
    void pause();


private slots:
    void onPlay();
    void onPrev();
    void onNext();
    void onMute();
    void onSpeedChanged();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onSeek(int seconds);
    void setState(QMediaPlayer::State state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    Ui::VideoPlayer ui;

    QMediaPlayer*   _player;
    QString         _filePath;
    bool            _muted;
    QMediaPlayer::State _state;
    int             _toSeek;
};

#endif // VIDEOPLAYER_H
