#ifndef PIPELINE_H
#define PIPELINE_H

#include "Camera.h"
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QList>

class FrameHandler;

/**
 * A pipeline contains multiple FramHanders
 * A frame will be processed by the FrameHandlers in the order they are added to the pipeline
 */
class PipeLine: public QObject
{
    Q_OBJECT

public:
    void openCamera(const Camera& camera);
	void addHandler(FrameHandler* handler);
    void start(int fps = 10);
    void stop();
    cv::Size getFrameSize();
    Camera getCamera() const;

private slots:
    void onTimer();

private:
    cv::VideoCapture        _input;
    QList<FrameHandler*>    _handlers;
    QTimer                  _timer;
    cv::Mat                 _frame;
    cv::Mat                 _prevFrame;
    Camera                  _camera;
};

#endif
