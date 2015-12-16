#ifndef CAMERAPAGE_H
#define CAMERAPAGE_H

#include "PipeLine.h"
#include "ui_CameraPage.h"

class QGridLayout;
class CameraPagelet;

class CameraPage : public QWidget
{
    Q_OBJECT

public:
    CameraPage(QWidget* parent = 0);
    ~CameraPage();

private slots:
    void onMaximizeRequested(CameraPagelet* toBeMaximized, bool maximize);

private:
    Ui::CameraPage ui;
    QGridLayout*    _layout;
};

#endif // CAMERAPAGE_H
