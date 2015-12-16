#ifndef MotionDetecter_h__
#define MotionDetecter_h__

#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include "FrameHandler.h"

/**
 * Detecting motion from a given video frame
 */
class MotionDetector: public FrameHandler
{
public:
    MotionDetector();
    void setThreshold(int width, int height, int number);
    void handleFrame(cv::Mat& frame, cv::Mat& previous);

private:
    void detectMotion(cv::Mat& frame1, cv::Mat& frame2);

private:
	int _widthThreshold;
	int _heightThreshold;
    int _numberThreshold;
    cv::Mat _foreground;
    cv::Ptr<cv::BackgroundSubtractorMOG2> _substractor;
};

#endif // MotionDetecter_h__

