#include "CameraPagelet.h"
#include "MySettings.h"
#include "CameraConfigDlg.h"
#include "MotionDetecter.h"
#include "MySettings.h"
#include "PipeLine.h"
#include "VideoSaver.h"
#include "VideoViewer.h"

#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QDir>

CameraPagelet::CameraPagelet(int row, int col, QWidget* parent) :
    QWidget(parent), _row(row), _col(col)
{
    ui.setupUi(this);

    _btConfig = new QPushButton(QIcon(":/Images/Config.png"),   "", ui.label);
    _btDelete = new QPushButton(QIcon(":/Images/Delete.png"),   "", ui.label);
    _btTurnOn = new QPushButton(QIcon(":/Images/CameraOn.png"), "", ui.label);
    _btTurnOn->setCheckable(true);
    hideButtons();

    setMouseTracking(true); // for enterEvent and leaveEvent to work

    connect(_btConfig, SIGNAL(clicked()), this, SLOT(onSetCamera()));
    connect(_btDelete, SIGNAL(clicked()), this, SLOT(onDelCamera()));
    connect(_btTurnOn, SIGNAL(clicked(bool)), this, SLOT(onTurnOnCamera(bool)));

    _motionDetector = new MotionDetector();
    _videoViewer    = new VideoViewer(ui.label);
    _videoSaver     = new VideoSaver;
    _pipeLine.addHandler(_motionDetector);
    _pipeLine.addHandler(_videoViewer);
    _pipeLine.addHandler(_videoSaver);
}

void CameraPagelet::setCamera(const Camera& camera)
{
    _camera = camera;

    // camera doesn't know its location
    _camera.row = _row;
    _camera.col = _col;

    ui.groupBox->setTitle(camera.name);
    onTurnOnCamera(false);
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

void CameraPagelet::resizeEvent(QResizeEvent*) {
    _videoViewer->resize(ui.label->width(), ui.label->height());
}

void CameraPagelet::showButtons()
{
    _btConfig->show();
    _btTurnOn->show();
    _btDelete->show();

    _btTurnOn->setEnabled(_camera.isValid());
    _btDelete->setEnabled(_camera.isValid());

    const int yMargin   = 70;
    const int y         = ui.label->height() - yMargin;
    const int centerX   = ui.label->width() / 2;
    const int space     = 10;

    _btTurnOn->move(centerX - _btTurnOn->width()/2, y);
    _btConfig->move(centerX - _btTurnOn->width()/2 - space - _btConfig->width(), y);
    _btDelete->move(centerX + _btTurnOn->width()/2 + space, y);
}

void CameraPagelet::hideButtons()
{
    _btConfig->hide();
    _btTurnOn->hide();
    _btDelete->hide();
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
    _btTurnOn->setIcon(on ? QIcon(":/Images/CameraOff.png")
                          : QIcon(":/Images/CameraOn.png"));
    _btTurnOn->setChecked(on);

    if(!on)
    {
        _pipeLine.stop();
        ui.label->setText("Camera is off");
    }
    else
    {
        if (_camera.url.isEmpty())
            _pipeLine.openDevice(0);
        else
            _pipeLine.openDevice(_camera.url);

        _videoViewer->setOriginalFrameSize(_pipeLine.getFrameSize());

        MySettings settings;
        _motionDetector->setThreshold(settings.getMinWidth(),
                                      settings.getMinHeight(),
                                      settings.getMinNumber());

        QString path = settings.getStoragePath();
        if (path.isEmpty())
            path = ".";
        QDir(path).mkdir(_camera.name);
        _videoSaver->config(path + "/" + _camera.name,
                            VideoWriter::fourcc('D', 'I', 'V', 'X'),
                            settings.getFPS(),
                            _pipeLine.getFrameSize(),
                            settings.getStorageInterval());

        _pipeLine.start(settings.getFPS());

        hideButtons();
    }
}

void CameraPagelet::onDelCamera()
{
    if (QMessageBox::warning(this, tr("Delete camera"), tr("Are you sure to delete this camera?"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        onTurnOnCamera(false);
        _camera.clear();
        ui.label->setText("No camera");
        ui.groupBox->setTitle(tr("No camera"));
    }
}
