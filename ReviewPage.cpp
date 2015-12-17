#include "Camera.h"
#include "MotionManager.h"
#include "MySettings.h"
#include "ReviewPage.h"

ReviewPage::ReviewPage(QWidget* parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    onToday();

    MySettings settings;
    QList<Camera> cameras = settings.getCameras();
    foreach(const Camera& camera, cameras)
        ui.comboCamera->addItem(camera.name);
    ui.comboCamera->setCurrentIndex(0);

    ui.timeLine->setBackgroundColor(Qt::lightGray);
    ui.timeLine->addEventSerie(1, Qt::green);
    ui.timeLine->addEventSerie(2, Qt::blue);

    connect(ui.comboCamera, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTimeLine()));
    connect(ui.btToday,     SIGNAL(clicked()), this, SLOT(onToday()));
    connect(ui.btPrevHour,  SIGNAL(clicked()), this, SLOT(onPrevHour()));
    connect(ui.btNextHour,  SIGNAL(clicked()), this, SLOT(onNextHour()));
    connect(ui.btPrevDay,   SIGNAL(clicked()), this, SLOT(onPrevDay()));
    connect(ui.btNextDay,   SIGNAL(clicked()), this, SLOT(onNextDay()));
    connect(ui.sbRange,     SIGNAL(valueChanged(int)), this, SLOT(onRangeChanged()));
    connect(ui.timeLine,    SIGNAL(mouseClicked(QDateTime)), this, SLOT(onPlayVideo(QDateTime)));

    updateTimeLine();
}

void ReviewPage::onToday()
{
    QDate today = QDate::currentDate();
    QDateTime start = QDateTime(today, QTime(0, 0));
    ui.dateEdit->setDate(today);
    ui.timeLine->setRange(start, start.addSecs(3600 * ui.sbRange->value()));
    updateTimeLine();
}

void ReviewPage::onPrevHour() {
    moveRangeBy(-1);
}

void ReviewPage::onNextHour() {
    moveRangeBy(1);
}

void ReviewPage::onPrevDay() {
    moveRangeBy(-24);
}

void ReviewPage::onNextDay() {
    moveRangeBy(24);
}

void ReviewPage::onRangeChanged()
{
    moveRangeBy(0);
}

void ReviewPage::onPlayVideo(const QDateTime& time)
{
    Video video = MotionManager::getInstance().getVideo(ui.comboCamera->currentText(), time.toMSecsSinceEpoch());
    if (!video._filePath.isEmpty())
    {
        ui.player->play(video._filePath, video._start.secsTo(time));
//        ui.player->seek(video._start.secsTo(time));
    }
}

void ReviewPage::moveRangeBy(int hours)
{
    QDateTime time = ui.timeLine->getStart().addSecs(hours * 3600);
    ui.dateEdit->setDate(time.date());
    ui.timeLine->setRange(time, time.addSecs(3600 * ui.sbRange->value()));
    updateTimeLine();
}

void ReviewPage::updateTimeLine()
{
    QString cameraName = ui.comboCamera->currentText();
    qint64 start = ui.timeLine->getStart().toMSecsSinceEpoch();
    qint64 end   = ui.timeLine->getEnd  ().toMSecsSinceEpoch();

    QList<Video> videos = MotionManager::getInstance().getVideos(cameraName, start, end);
    foreach(const Video& video, videos)
        ui.timeLine->addEvent(1, video._start, video._end);

    QList<Motion> motions = MotionManager::getInstance().getMotions(cameraName, start, end);
    foreach(const Motion& motion, motions)
        ui.timeLine->addEvent(2, motion._start, motion._end);
}
