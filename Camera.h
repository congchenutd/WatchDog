#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QJsonObject>

/**
 * Camera information
 */
struct Camera
{
    int row;
    int col;
    QString name;
    QString url;

    static Camera fromJson(const QJsonObject& jsonObj);
    QJsonObject toJson() const;
    void clear();
    bool isValid() const;
};

#endif // CAMERA_H
