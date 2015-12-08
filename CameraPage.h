#ifndef CAMERAPAGE_H
#define CAMERAPAGE_H

#include "PipeLine.h"
#include "ui_CameraPage.h"

class CameraPage : public QWidget
{
    Q_OBJECT

public:
    CameraPage(QWidget* parent = 0);

private slots:
    void onTurnOn(bool on);

private:
    Ui::CameraPage ui;
    PipeLine _pipeLine;
};

#endif // CAMERAPAGE_H
