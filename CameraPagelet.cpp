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
#include <QDebug>
#include <QMouseEvent>

CameraPagelet::CameraPagelet(int row, int col, QWidget* parent) :
    QWidget(parent), _row(row), _col(col)
{
    ui.setupUi(this);

    _btConfig   = new QPushButton(QIcon(":/Images/Config.png"),   "", ui.label);
    _btDelete   = new QPushButton(QIcon(":/Images/Delete.png"),   "", ui.label);
    _btTurnOn   = new QPushButton(QIcon(":/Images/CameraOn.png"), "", ui.label);
    _btMaximize = new QPushButton(QIcon(":/Images/Maximize.png"), "", ui.label);
    _btConfig   ->setIconSize(QSize(28, 28));
    _btDelete   ->setIconSize(QSize(28, 28));
    _btTurnOn   ->setIconSize(QSize(28, 28));
    _btMaximize ->setIconSize(QSize(28, 28));
    _btTurnOn  ->setCheckable(true);
    _maximized = false;
    hideButtons();

    QPushButton* showAll = new QPushButton(this);
    showAll->resize(0, 0);
    showAll->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(showAll, SIGNAL(clicked()), this, SLOT(showAllCameras()));

    connect(_btConfig,   SIGNAL(clicked()), this, SLOT(onSetCamera()));
    connect(_btDelete,   SIGNAL(clicked()), this, SLOT(onDelCamera()));
    connect(_btMaximize, SIGNAL(clicked()), this, SLOT(toggleMaximized()));
    connect(_btTurnOn,   SIGNAL(clicked(bool)), this, SLOT(onTurnOnCamera(bool)));
    connect(&_idleTimer, SIGNAL(timeout()), this, SLOT(hideButtons()));

    _motionDetector = new MotionDetector();
    _videoViewer    = new VideoViewer(ui.label);
    _videoSaver     = new VideoSaver;
    _pipeLine.addHandler(_motionDetector);
    _pipeLine.addHandler(_videoViewer);
    _pipeLine.addHandler(_videoSaver);

    ui.label->setMouseTracking(true);
    ui.label->installEventFilter(this);
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

void CameraPagelet::resizeEvent(QResizeEvent*)
{
    _videoViewer->resize(ui.label->width(), ui.label->height());
    updateButtons();
}

bool CameraPagelet::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui.label && _camera.isValid())
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            toggleMaximized();
            return true;
        }
        else if (event->type() == QEvent::MouseMove)
        {
            showButtons();
            return true;
        }
    }
    return false;
}

void CameraPagelet::updateButtons()
{
    const int bottomMargin = 40;
    const int y            = ui.label->height() - bottomMargin;
    const int space        = 10;
    const int buttonWidth  = _btConfig->width();
    const int totalWidth   = _camera.isValid() ? 4 * buttonWidth + 3 * space : buttonWidth;
    int x = (ui.label->width() - totalWidth) / 2;

    _btConfig->move(x, y);
    x += buttonWidth + space;
    _btTurnOn->move(x, y);
    x += buttonWidth + space;
    _btDelete->move(x, y);
    x += buttonWidth + space;
    _btMaximize->move(x, y);
}

void CameraPagelet::showButtons()
{
    _btConfig  ->show();
    _btTurnOn  ->setVisible(_camera.isValid());
    _btDelete  ->setVisible(_camera.isValid());
    _btMaximize->setVisible(_camera.isValid());

    updateButtons();
    _idleTimer.start(5000);
}

void CameraPagelet::hideButtons()
{
    _btConfig  ->hide();
    _btTurnOn  ->hide();
    _btDelete  ->hide();
    _btMaximize->hide();
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
    _btTurnOn->setIcon(QIcon(on ? ":/Images/CameraOff.png"
                                : ":/Images/CameraOn.png"));
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

void CameraPagelet::toggleMaximized()
{
    _maximized = !_maximized;
    doMaximize(_maximized);
}

void CameraPagelet::showAllCameras() {
    doMaximize(false);
}

void CameraPagelet::doMaximize(bool maximize)
{
    _maximized = maximize;
    _btMaximize->setIcon(QIcon(maximize ? ":/Images/AllCameras.png"
                                        : ":/Images/Maximize.png"));
    emit maximizeRequested(this, maximize);
}
