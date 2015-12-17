#include "MotionManager.h"
#include "MySettings.h"
#include "Camera.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>

MotionManager::MotionManager()
{
    _motionStart    = -1;
    _motionEnd      = -1;
    _videoStart     = -1;
    _videoEnd       = -1;
    createDB();
}

MotionManager& MotionManager::getInstance()
{
    static MotionManager instance;
    return instance;
}

/**
 * Add a motion to the DB
 * Will be merged with the previous motion if they are closer than MergeThreshold
 */
void MotionManager::addMotion(const Camera& camera)
{
    qint64 time = QDateTime::currentMSecsSinceEpoch();

    // first motion
    if (_motionEnd == -1)
    {
        _motionStart = time;
        _motionEnd   = time;
    }
    else
    {
        // not far enough from previous motion, merge
        if (time - _motionEnd <= MergeThreshold) {
            _motionEnd = time;
        }
        else
        {
            // save previous motion
            QSqlQuery query;
            query.exec(tr("insert into Motions values(%1, %2, %3, \"%4\")")
                       .arg(getNextID("Motions"))
                       .arg(_motionStart)
                       .arg(_motionEnd)
                       .arg(camera.name));

            _motionStart = time;
            _motionEnd   = time;
        }
    }
}

/**
 * Add a video to the DB
 * Will be merged with the previous motion if they are closer than MergeThreshold
 */
void MotionManager::addVideo(const QString& filePath, const Camera& camera)
{
    qint64 time = QDateTime::currentMSecsSinceEpoch();

    if (_videoEnd == -1)
    {
        _videoStart = time;
        _videoEnd   = time;
    }
    else
    {
        // merge if too close
        if (time - _videoEnd <= MergeThreshold) {
            _videoEnd = time;
        }
        else
        {
            // write into DB
            MySettings settings;
            QSqlQuery query;
            query.exec(tr("insert into Videos values(%1, %2, %3, \"%4\", \"%5\")")
                       .arg(getNextID("Videos"))
                       .arg(time)
                       .arg(time + settings.getStorageInterval() * 60000)
                       .arg(filePath)
                       .arg(camera.name));

            _videoStart = time;
            _videoEnd   = time;
        }
    }

}

/**
 * Find a video that includes the given time
 * @param cameraName    - from which camera
 * @param time          - the time in msec that is included by the video
 * @return              - a video object
 */
Video MotionManager::getVideo(const QString& cameraName, qint64 time) const
{
    QSqlQuery query;
    query.exec(tr("select Start, End, FilePath from Videos where Camera = \"%1\" and Start <= %2 and %2 <= End")
               .arg(cameraName).arg(time));
    return query.next() ? Video(query.value(0).toLongLong(),
                                query.value(1).toLongLong(),
                                query.value(2).toString())
                        : Video(-1, -1, QString());
}

/**
 * Get all the videos withing a given time period
 */
QList<Video> MotionManager::getVideos(const QString& cameraName, qint64 start, qint64 end) const
{
    QList<Video> result;
    QSqlQuery query;
    query.exec(tr("select Start, End, FilePath from Videos where Camera = \"%1\" and %2 <= Start and End <= %3")
               .arg(cameraName).arg(start).arg(end));
    while (query.next())
        result << Video(query.value(0).toLongLong(), query.value(1).toLongLong(), query.value(2).toString());
    return result;
}

/**
 * Get all the motions withing a given time period
 */
QList<Motion> MotionManager::getMotions(const QString& cameraName, qint64 start, qint64 end) const
{
    QList<Motion> result;
    QSqlQuery query;
    query.exec(tr("select Start, End from Motions where Camera = \"%1\" and Start >= %2 and End <= %3")
               .arg(cameraName).arg(start).arg(end));
    while (query.next())
        result << Motion(query.value(0).toLongLong(), query.value(1).toLongLong());
    return result;
}

void MotionManager::createDB()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("Motions.db");
    if(!database.open())
    {
        QMessageBox::critical(0, tr("Error"), tr("Can not open database"));
        return;
    }

    QSqlQuery query;
    query.exec("create table Videos( \
                   ID       int primary key, \
                   Start    date,       \
                   End      date,       \
                   FilePath varchar,    \
                   Camera   varchar     \
               )");
    query.exec("create table Motions( \
                   ID       int primary key, \
                   Start    date,   \
                   End      date,   \
                   Camera   varchar \
               )");
}

int MotionManager::getNextID(const QString& tableName) const
{
    QSqlQuery query;
    query.exec(tr("select max(ID) from %1").arg(tableName));
    return query.next() ? query.value(0).toInt() + 1 : 1;
}

Video::Video(qint64 start, qint64 end, const QString& path)
{
    _start      = QDateTime::fromMSecsSinceEpoch(start);
    _end        = QDateTime::fromMSecsSinceEpoch(end);
    _filePath   = path;
}

Motion::Motion(qint64 start, qint64 end)
{
    _start  = QDateTime::fromMSecsSinceEpoch(start);
    _end    = QDateTime::fromMSecsSinceEpoch(end);
}
