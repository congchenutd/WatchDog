#ifndef CAMERAPAGELET_H
#define CAMERAPAGELET_H

#include "ui_CameraPagelet.h"
#include "Camera.h"
#include "PipeLine.h"

class QPushButton;
class MotionDetector;
class VideoViewer;
class VideoSaver;

/**
 * A pagelet holds a camera and related buttons
 */
class CameraPagelet : public QWidget
{
    Q_OBJECT

public:
    explicit CameraPagelet(int row, int col, QWidget *parent = 0);
    void setCamera(const Camera& camera);
    Camera getCamera() const;

protected:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void resizeEvent(QResizeEvent*);

private slots:
    void showButtons();
    void hideButtons();
    void onSetCamera();
    void onTurnOnCamera(bool on);
    void onDelCamera();

private:
    Ui::CameraPagelet ui;

    Camera      _camera;
    int         _row;
    int         _col;

    PipeLine        _pipeLine;
    MotionDetector* _motionDetector;
    VideoViewer*    _videoViewer;
    VideoSaver*     _videoSaver;

    QPushButton*    _btConfig;
    QPushButton*    _btDelete;
    QPushButton*    _btTurnOn;
};

#endif // CAMERAPAGELET_H
