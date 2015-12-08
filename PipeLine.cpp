#include "PipeLine.h"
#include "FrameHandler.h"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;

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

Size PipeLine::getFrameSize() {
    return Size(_input.get(CAP_PROP_FRAME_WIDTH),
                _input.get(CAP_PROP_FRAME_HEIGHT));
}

void PipeLine::onTimer()
{
    if(!_input.isOpened())
        return;

    _input >> _frame;
    foreach(FrameHandler* handler, _handlers)
        handler->handleFrame(_frame);
}

void PipeLine::openDevice(int device) {
    _input.open(device);
}

void PipeLine::openDevice(const String& device) {
    _input.open(device);
}

void PipeLine::addHandler(FrameHandler* handler) {
	_handlers.push_back(handler);
}
