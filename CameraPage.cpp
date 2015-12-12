#include "CameraPage.h"
#include "MySettings.h"
#include "CameraPagelet.h"

#include <QGridLayout>
#include <QLayoutItem>

CameraPage::CameraPage(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    // read grid size
    MySettings settings;
    int nRow = settings.getGridSize().first;
    int nCol = settings.getGridSize().second;

    // create a grid layout
    _layout = new QGridLayout(this);
    _layout->setMargin(0);
    _layout->setSpacing(5);
    for (int row = 0; row < nRow; ++row)
        for (int col = 0; col < nCol; ++col)
        {
            // create a page let for each grid cell
            CameraPagelet* pagelet = new CameraPagelet(row, col, this);
            _layout->addWidget(pagelet, row, col);
        }

    // load cameras
    foreach(const Camera& camera, settings.getCameras()) {
        if (QLayoutItem* item = _layout->itemAtPosition(camera.row, camera.col))
        {
            CameraPagelet* pagelet = (CameraPagelet*) item->widget();
            pagelet->setCamera(camera);
        }
    }
}

/**
 * Save camera info
 */
CameraPage::~CameraPage()
{
    QList<Camera> cameras;
    for (int row = 0; row < _layout->rowCount(); ++row)
        for (int col = 0; col < _layout->columnCount(); ++col)
        {
            // get the camera in this cell
            CameraPagelet* pagelet = (CameraPagelet*) _layout->itemAtPosition(row, col)->widget();
            Camera camera = pagelet->getCamera();

            // if the camera is configured
            if (camera.isValid())
                cameras << camera;
        }

    // save the cameras
    MySettings settings;
    settings.saveCameras(cameras);
}

