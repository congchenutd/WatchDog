#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "ui_VideoPlayer.h"
#include <QMediaPlayer>

class QMediaPlaylist;

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget* parent = 0);
    void play(const QString& filePath);
    void pause();
    void addFile(const QString& filePath);
    void setState(QMediaPlayer::State state);

private slots:
    void onPlay();
    void onStop();
    void onPrev();
    void onNext();
    void onMute();
    void onProgress(int);

private:
    Ui::VideoPlayer ui;

    QMediaPlayer*   _player;
    QMediaPlaylist* _playList;
    QString         _filePath;
    bool            _muted;
    QMediaPlayer::State _state;
};

#endif // VIDEOPLAYER_H
