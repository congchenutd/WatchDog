#include "PipeLine.h"
#include "FrameHandler.h"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

void PipeLine::start(int fps)
{
    connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer.start(qMax(10, 1000 / fps));
}

void PipeLine::stop()
{
    _input.release();
    _timer.stop();
}

cv::Size PipeLine::getFrameSize() {
    return cv::Size(_input.get(cv::CAP_PROP_FRAME_WIDTH),
                    _input.get(cv::CAP_PROP_FRAME_HEIGHT));
}

Camera PipeLine::getCamera() const {
    return _camera;
}

void PipeLine::onTimer()
{
    if(!_input.isOpened())
        return;

    _input >> _prevFrame >> _frame;
    foreach(FrameHandler* handler, _handlers)
        handler->handleFrame(_frame, _prevFrame);
}

void PipeLine::openCamera(const Camera& camera)
{
    _camera = camera;
    if (camera.url.isEmpty())
        _input.open(0);
    else
        _input.open(camera.url.toStdString());
}

void PipeLine::addHandler(FrameHandler* handler) {
	_handlers.push_back(handler);
    handler->setPipeLine(this);
}
