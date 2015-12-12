#include "Camera.h"

Camera Camera::fromJson(const QJsonObject& jsonObj)
{
    Camera camera;
    camera.row  = jsonObj.value("row") .toInt();
    camera.col  = jsonObj.value("col") .toInt();
    camera.name = jsonObj.value("name").toString();
    camera.url  = jsonObj.value("url") .toString();
    return camera;
}

QJsonObject Camera::toJson() const
{
    QJsonObject result;
    result.insert("row",    row);
    result.insert("col",    col);
    result.insert("name",   name);
    result.insert("url",    url);
    return result;
}

void Camera::clear()
{
    row = -1;
    col = -1;
    name.clear();
    url.clear();
}

bool Camera::isValid() const {
    return !name.isEmpty();
}
