#ifndef FrameHandler_h__
#define FrameHandler_h__

#include <opencv2/opencv.hpp>

class FrameHandler
{
public:
	virtual void handleFrame(cv::Mat& frame) = 0;
};
#endif // FrameHandler_h__

