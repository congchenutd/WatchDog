#include "MotionDetecter.h"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <vector>

using namespace cv;
using namespace std;
	   
MotionDetector::MotionDetector()
{
    setThreshold(100, 100, 1);
    _substractor = createBackgroundSubtractorMOG2();
}

void MotionDetector::setThreshold(int width, int height, int number)
{
    _widthThreshold  = width;
    _heightThreshold = height;
    _numberThreshold = number;
}

void MotionDetector::handleFrame(Mat& frame, Mat& previous)
{
    detectMotion(previous, frame);
}

void MotionDetector::detectMotion(Mat& frame1, Mat& frame2)
{
    // Blur images to reduce noise
    Mat blurred1, blurred2;
    blur(frame1, blurred1, Size(4 ,4));
    blur(frame2, blurred2, Size(4, 4));

    // Get absolute difference image
    Mat diff;
    absdiff(blurred1, blurred2, diff);

    // Split image to each channels
    vector<Mat> channels;
    split(diff, channels);

    // Apply threshold to each channel and combine the results
    Mat thresholds = Mat::zeros(diff.size(), CV_8UC1);
    for (int i = 0; i < channels.size(); i++)
    {
        Mat thresh;
        threshold(channels[i], thresh, 45, 255, CV_THRESH_BINARY);
        thresholds |= thresh;
    }

    // Perform morphological close operation to filling in the gaps
    Mat e;
    getStructuringElement(MORPH_RECT, Size(10, 10));
    morphologyEx(thresholds, e, MORPH_CLOSE, Mat(), Point(-1,-1), 5);

    // Find all contours
//    vector<vector<Point> > contours;
//    findContours(e, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

//    const Scalar redColor = Scalar(0, 0, 255);
//    for(size_t i = 0; i < contours.size(); i++)
//    {
//        // Approximate contours to polygons
//        vector<Point> polygon;
//        approxPolyDP(Mat(contours[i]), polygon, 3, true);

//        // Draw bounding rectangle
//        Rect boundRect = boundingRect(Mat(polygon));
//        if (boundRect.width > 100 && boundRect.height > 100)
//            rectangle(frame2, boundRect.tl(), boundRect.br(), redColor, 2, 8, 0);
//    }
}


