#include "MotionManager.h"
#include "MotionDetecter.h"
#include "PipeLine.h"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <QDateTime>

using cv::Mat;
using cv::Size;
using cv::Point;
using cv::Scalar;
using cv::Rect;
using namespace std;

MotionDetector::MotionDetector()
{
    setThreshold(100, 100, 1);
    _substractor = cv::createBackgroundSubtractorMOG2();
}

void MotionDetector::setThreshold(int width, int height, int number)
{
    _widthThreshold  = width;
    _heightThreshold = height;
    _numberThreshold = number;
}

void MotionDetector::handleFrame(cv::Mat& frame, cv::Mat& previous)
{
    detectMotion(frame, previous);
}

void MotionDetector::detectMotion(cv::Mat& frame, cv::Mat& previous)
{
    const Scalar color = Scalar(0, 0, 255);
    vector<vector<Point> > contours;

    _substractor->apply(frame, _foreground);
    erode (_foreground, _foreground, Mat());
    dilate(_foreground, _foreground, Mat());
    findContours(_foreground, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    if(contours.size() < _numberThreshold)
        return;

    int objectCount = 0;
    for(size_t i = 0; i < contours.size(); i++)
    {
        // Approximate contours to polygons
        vector<Point> polygon;
        approxPolyDP(Mat(contours[i]), polygon, 3, true);

        // Draw bounding rectangle
        Rect boundRect = boundingRect(Mat(polygon));
        if (boundRect.width > _widthThreshold && boundRect.height > _heightThreshold)
        {
            rectangle(frame, boundRect.tl(), boundRect.br(), color, 2, 8, 0);
            objectCount ++;
        }
    }

    if (objectCount > 0)
        MotionManager::getInstance().addMotion(_pipeLine->getCamera());
}


