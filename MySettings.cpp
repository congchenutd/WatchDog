#include "MySettings.h"

MySettings::MySettings()
    : QSettings("SleepyBoy.ini", QSettings::IniFormat)
{

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




