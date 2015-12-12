#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "ui_CameraView.h"
#include "PipeLine.h"

class MotionDetector;
class VideoViewer;
class VideoSaver;

/**
 * A camera viewer
 */
class CameraView : public QWidget
{
    Q_OBJECT

public:
    explicit CameraView(const QString& url, QWidget* parent = 0);
    void turnOn(bool on);

private:
    Ui::CameraView ui;

    QString         _url;
    PipeLine        _pipeLine;
    MotionDetector* _motionDetector;
    VideoViewer*    _videoViewer;
    VideoSaver*     _videoSaver;
};

#endif // CAMERAVIEW_H
