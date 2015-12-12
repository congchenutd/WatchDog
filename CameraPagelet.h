#ifndef CAMERAPAGELET_H
#define CAMERAPAGELET_H

#include "ui_CameraPagelet.h"
#include "Camera.h"

class CameraView;

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

private slots:
    void showButtons();
    void hideButtons();
    void onSetCamera();
    void onTurnOnCamera(bool on);
    void onDelCamera();

private:
    Ui::CameraPagelet ui;

    CameraView* _cameraView;
    Camera      _camera;
    int         _row;
    int         _col;
};

#endif // CAMERAPAGELET_H
