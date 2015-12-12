#include "CameraPagelet.h"
#include "MySettings.h"
#include "CameraConfigDlg.h"
#include "CameraView.h"
#include <QTimer>
#include <QMessageBox>

CameraPagelet::CameraPagelet(int row, int col, QWidget* parent) :
    QWidget(parent), _cameraView(0), _row(row), _col(col)
{
    ui.setupUi(this);
    setMouseTracking(true); // for enterEvent and leaveEvent to work
    hideButtons();
    connect(ui.btConfig, SIGNAL(clicked()), this, SLOT(onSetCamera()));
    connect(ui.btDelete, SIGNAL(clicked()), this, SLOT(onDelCamera()));
    connect(ui.btOn, SIGNAL(clicked(bool)), this, SLOT(onTurnOnCamera(bool)));
}

void CameraPagelet::setCamera(const Camera& camera)
{
    _camera = camera;

    // camera doesn't know its location
    _camera.row = _row;
    _camera.col = _col;

    _cameraView = new CameraView(camera.url, this);
    ui.groupBox->layout()->addWidget(_cameraView);
    ui.groupBox->setTitle(camera.name);
}

Camera CameraPagelet::getCamera() const {
    return _camera;
}

void CameraPagelet::enterEvent(QEvent*) {
    QTimer::singleShot(250, this, SLOT(showButtons()));
}

void CameraPagelet::leaveEvent(QEvent*) {
    QTimer::singleShot(250, this, SLOT(hideButtons()));
}

void CameraPagelet::showButtons()
{
    ui.btConfig->show();
    if (_cameraView != 0)
    {
        ui.btOn->show();
        ui.btDelete->show();
    }
}

void CameraPagelet::hideButtons()
{
    ui.btConfig->hide();
    ui.btOn->hide();
    ui.btDelete->hide();
}

void CameraPagelet::onSetCamera()
{
    CameraConfigDlg dlg(this);
    dlg.setCamera(_camera);
    if (dlg.exec() == QDialog::Accepted)
        setCamera(dlg.getCamera());
}

void CameraPagelet::onTurnOnCamera(bool on)
{
    _cameraView->turnOn(on);
    ui.btOn->setIcon(on ? QIcon(":/Images/CameraOff.png") : QIcon(":/Images/CameraOn.png"));
}

void CameraPagelet::onDelCamera()
{
    if (QMessageBox::warning(this, tr("Delete camera"), tr("Are you sure to delete this camera?"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        delete _cameraView;
        _cameraView = 0;
        _camera.clear();
        ui.groupBox->setTitle(tr("No camera"));
    }
}
