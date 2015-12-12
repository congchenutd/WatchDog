#include "MySettings.h"
#include "Camera.h"
#include "QJsonObject"
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>

MySettings::MySettings()
    : QSettings("SleepyBoy.ini", QSettings::IniFormat)
{
    if (QFile("SleepyBoy.ini").size() == 0)
        createDefaults();
}

void MySettings::createDefaults()
{
    setFPS(5);
    setMinWidth(100);
    setMinHeight(100);
    setMinNumber(1);
    setStorageInterval(1);
    setStoragePath(QDir::currentPath());
    setMaxStorage(10);
    setMaxDays(7);
    setGridSize(2, 2);
}

void MySettings::setUrl(const QString& url) {
    setValue("Url", url);
}

void MySettings::setFPS(int fps) {
    setValue("FPS", fps);
}

void MySettings::setMinWidth(int width) {
    setValue("MinWidth", width);
}

void MySettings::setMinHeight(int height) {
    setValue("MinHeight", height);
}

void MySettings::setMinNumber(int number) {
    setValue("MinNumber", number);
}

void MySettings::setStorageInterval(int minutes) {
    setValue("StorageInterval", minutes);
}

void MySettings::setStoragePath(const QString& path) {
    setValue("StoragePath", path);
}

void MySettings::setMaxStorage(int gb) {
    setValue("MaxStorage", gb);
}

void MySettings::setMaxDays(int days) {
    setValue("MaxDays", days);
}

void MySettings::setGridSize(int nRow, int nCol)
{
    setValue("GridRowCount", nRow);
    setValue("GridColCount", nCol);
}

void MySettings::saveCameras(const QList<Camera>& cameras)
{
    QJsonArray jsonArray;
    foreach(const Camera& camera, cameras)
        jsonArray << camera.toJson();

    // FIXME: setValue("cameras", jsonArray) will throw an error
    // So I convert jsonArray to text as a QByteArray
    setValue("cameras", QJsonDocument(jsonArray).toJson());
}

QString MySettings::getUrl() const {
    return value("Url").toString();
}

int MySettings::getFPS() const {
    return value("FPS").toInt();
}

int MySettings::getMinWidth() const {
    return value("MinWidth").toInt();
}

int MySettings::getMinHeight() const {
    return value("MinHeight").toInt();
}

int MySettings::getMinNumber() const {
    return value("MinNumber").toInt();
}

int MySettings::getStorageInterval() const {
    return value("StorageInterval").toInt();
}

QString MySettings::getStoragePath() const {
    return value("StoragePath").toString();
}

int MySettings::getMaxStorage() const {
    return value("MaxStorage").toInt();
}

int MySettings::getMaxDays() const {
    return value("MaxDays").toInt();
}

QPair<int, int> MySettings::getGridSize() const {
    return QPair<int, int>(value("GridRowCount").toInt(),
                           value("GridColCount").toInt());
}

QList<Camera> MySettings::getCameras() const
{
    QList<Camera> result;
    QJsonArray jsonArray = QJsonDocument::fromJson(value("Cameras").toByteArray()).array();
    foreach(const QJsonValue& value, jsonArray)
        result << Camera::fromJson(value.toObject());
    return result;
}




