#ifndef MOTIONDB_H
#define MOTIONDB_H

#include <QDateTime>
#include <QObject>
#include <QPair>

class Camera;

struct Video
{
    QDateTime   _start;
    QDateTime   _end;
    QString     _filePath;

    Video(qint64 start, qint64 end, const QString& path);
};

struct Motion
{
    QDateTime   _start;
    QDateTime   _end;

    Motion(qint64 start, qint64 end);
};

/**
 * Deals with DB
 */
class MotionManager: public QObject
{
public:
    static MotionManager& getInstance();
    void addMotion(const Camera& camera);
    void addVideo(const QString& filePath, const Camera& camera);
    Video           getVideo(const QString& cameraName, qint64 time) const;
    QList<Video>    getVideos   (const QString& cameraName, qint64 start, qint64 end) const;
    QList<Motion>   getMotions  (const QString& cameraName, qint64 start, qint64 end) const;

private:
    MotionManager();
    void createDB();
    int getNextID(const QString& tableName) const;

private:
    const static int MergeThreshold = 10000; // 10 seconds
    qint64  _motionStart;
    qint64  _motionEnd;
    qint64  _videoStart;
    qint64  _videoEnd;
};

#endif // MOTIONDB_H
