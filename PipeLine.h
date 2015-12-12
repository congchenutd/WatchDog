#ifndef PIPELINE_H
#define PIPELINE_H

#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QList>

class FrameHandler;
using namespace cv;

/**
 * A pipeline contains multiple FramHanders
 * A frame will be processed by the FrameHandlers in the order they are added to the pipeline
 */
class PipeLine: public QObject
{
    Q_OBJECT

public:
    void openDevice(int device);
    void openDevice(const QString& url);
	void addHandler(FrameHandler* handler);
    void start(int fps = 10);
    void stop();
    Size getFrameSize();

private slots:
    void onTimer();

private:
    VideoCapture            _input;
    QList<FrameHandler*>    _handlers;
    QTimer                  _timer;
    Mat                     _frame;
};

#endif
