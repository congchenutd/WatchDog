#ifndef CAMERACONFIGDLG_H
#define CAMERACONFIGDLG_H

#include "ui_CameraConfigDlg.h"

struct Camera;

/**
 * A dialog for config camera info
 */
class CameraConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CameraConfigDlg(QWidget* parent = 0);

    Camera getCamera() const;
    void setCamera(const Camera& camera);

    void accept();

private:
    Ui::CameraConfigDlg ui;
};

#endif // CAMERACONFIGDLG_H
