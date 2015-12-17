#include "CameraConfigDlg.h"
#include "Camera.h"

CameraConfigDlg::CameraConfigDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

Camera CameraConfigDlg::getCamera() const
{
    Camera camera;
    camera.name = ui.leName->text();
    camera.url  = ui.leUrl ->text();
    return camera;
}

void CameraConfigDlg::setCamera(const Camera& camera)
{
    ui.leName->setText(camera.name);
    ui.leUrl ->setText(camera.url);
}

void CameraConfigDlg::accept()
{
    if (!ui.leName->text().isEmpty())   // name must be none-empty
        return QDialog::accept();
}
