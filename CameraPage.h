#ifndef CAMERAPAGE_H
#define CAMERAPAGE_H

#include "PipeLine.h"
#include "ui_CameraPage.h"

class MotionDetector;
class VideoViewer;
class VideoSaver;
class QGridLayout;

class CameraPage : public QWidget
{
    Q_OBJECT

public:
    CameraPage(QWidget* parent = 0);
    ~CameraPage();

private:
    Ui::CameraPage ui;
    QGridLayout*    _layout;
};

#endif // CAMERAPAGE_H
