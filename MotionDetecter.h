#ifndef MotionDetecter_h__
#define MotionDetecter_h__

#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include "FrameHandler.h"

using namespace cv;

/**
 * Detecting motion from a given video frame
 */
class MotionDetecter: public FrameHandler
{
public:
	MotionDetecter();
    void setThreshold(int width, int height, int number);
	void handleFrame(Mat& frame);

private:
	int _widthThreshold;
	int _heightThreshold;
    int _numberThreshold;
	Mat _foreground;
	Ptr<BackgroundSubtractorMOG2> _substractor;
};

#endif // MotionDetecter_h__

