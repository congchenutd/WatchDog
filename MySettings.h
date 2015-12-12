#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

struct Camera;

class MySettings : public QSettings
{
public:
    MySettings();

    void setUrl             (const QString& url);
    void setFPS             (int fps);
    void setMinWidth        (int width);
    void setMinHeight       (int height);
    void setMinNumber       (int number);
    void setStorageInterval (int minutes);
    void setStoragePath     (const QString& path);
    void setMaxStorage      (int gb);
    void setMaxDays         (int days);
    void setGridSize        (int nRow, int nCol);
    void saveCameras        (const QList<Camera>& cameras);

    QString getUrl()                const;
    int     getFPS()                const;
    int     getMinWidth()           const;
    int     getMinHeight()          const;
    int     getMinNumber()          const;
    int     getStorageInterval()    const;
    QString getStoragePath()        const;
    int     getMaxStorage()         const;
    int     getMaxDays()            const;
    QPair<int, int> getGridSize()   const;
    QList<Camera> getCameras() const;

private:
    void createDefaults();
};

#endif // MYSETTINGS_H
