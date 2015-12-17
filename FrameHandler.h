#ifndef FrameHandler_h__
#define FrameHandler_h__

#include <opencv2/opencv.hpp>

class PipeLine;

/**
 * A worker on the pipe line
 */
class FrameHandler
{
public:
    virtual void handleFrame(cv::Mat& frame, cv::Mat& previous) = 0;
    void setPipeLine(PipeLine* pipeLine);

protected:
    PipeLine* _pipeLine;
};
#endif // FrameHandler_h__

