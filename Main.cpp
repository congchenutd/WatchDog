#include "MainWindow.h"
#include "MotionDetecter.h"
#include "PipeLine.h"
#include "VideoPlayer.h"
#include "VideoSaver.h"
#include "VideoViewer.h"
#include <QApplication>

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
/*
void intruderAlarm(cv::Mat& frame1, cv::Mat& frame2)
{
    // Blur images to reduce noise
    cv::Mat blurred1, blurred2;
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
    vector<vector<Point> > contours;
    findContours(e, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    const Scalar redColor = Scalar(0, 0, 255);
    for(size_t i = 0; i < contours.size(); i++)
    {
        // Approximate contours to polygons
        vector<Point> polygon;
        approxPolyDP(Mat(contours[i]), polygon, 3, true);

        // Draw bounding rectangle
        Rect boundRect = boundingRect(Mat(polygon));
        if (boundRect.width > 100 && boundRect.height > 100)
            rectangle(frame2, boundRect.tl(), boundRect.br(), redColor, 2, 8, 0);
    }
}


void foo()
{
    VideoCapture cap("http://192.168.1.9/axis-cgi/mjpg/video.cgi.mjpg");

    Mat frame, previous;
    while (1)
    {
        if(!cap.isOpened())
            return;

        cap.read(frame);
        Mat backup = frame.clone();
        if (previous.dims > 0)
            intruderAlarm(previous, frame);
        previous = backup.clone();
        imshow("Camera", frame);

        switch(waitKey(10))
        {
        case 27:
            return;
        }
    }

    return;
}*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

//    VideoPlayer player;
//    player.show();
//    player.play("a/1450305703153.avi", 30);

    return app.exec();
//    foo();
//    return 0;

//    PipeLine* pipeline = new PipeLine;
//    pipeline->openDevice("http://1:1@192.168.1.5:8888/videostream.asf");
//    QLabel* label = new QLabel();
//    pipeline->addHandler(new VideoViewer(label));
//    label->resize(640, 480);
//    label->show();
//    pipeline->start();

//    return app.exec();

//    VideoCapture cap("http://192.168.1.9/axis-cgi/mjpg/video.cgi.mjpg");
//    if (cap.isOpened())
//        return 0;
//    return 1;
}
