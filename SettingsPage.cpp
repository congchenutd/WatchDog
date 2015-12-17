#include "SettingsPage.h"
#include <QDateTime>
#include <QFileDialog>

SettingsPage::SettingsPage(QWidget* parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.btSave,          SIGNAL(clicked()), SLOT(onSave()));
    connect(ui.btStoragePath,   SIGNAL(clicked()), SLOT(onSetStoragePath()));
    enforceTimeLimit();
    enforceStorageLimit();
    loadSettings();
}

void SettingsPage::loadSettings()
{
    ui.sbFPS            ->setValue  (_settings.getFPS());
    ui.sbWidth          ->setValue  (_settings.getMinWidth());
    ui.sbHeight         ->setValue  (_settings.getMinHeight());
    ui.sbNumber         ->setValue  (_settings.getMinNumber());
    ui.sbStorageInterval->setValue  (_settings.getStorageInterval());
    ui.leStoragePath    ->setText   (_settings.getStoragePath());
    ui.sbMaxStorage     ->setValue  (_settings.getMaxStorage());
    ui.sbMaxDays        ->setValue  (_settings.getMaxDays());
    ui.sbGridNRows      ->setValue  (_settings.getGridSize().first);
    ui.sbGridNCols      ->setValue  (_settings.getGridSize().second);
}

void SettingsPage::onSave()
{
    _settings.setFPS            (ui.sbFPS->value());
    _settings.setMinWidth       (ui.sbWidth ->value());
    _settings.setMinHeight      (ui.sbHeight->value());
    _settings.setMinNumber      (ui.sbNumber->value());
    _settings.setStorageInterval(ui.sbStorageInterval->value());
    _settings.setStoragePath    (ui.leStoragePath->text());
    _settings.setMaxStorage     (ui.sbMaxStorage->value());
    _settings.setMaxDays        (ui.sbMaxDays->value());
    _settings.setGridSize(ui.sbGridNRows->value(), ui.sbGridNCols->value());
}

void SettingsPage::onSetStoragePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ".",  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
        ui.leStoragePath->setText(dir);
}

/**
 * Remove old videos
 */
void SettingsPage::enforceTimeLimit()
{
    QDir dir(_settings.getStoragePath());
    QFileInfoList infoList = dir.entryInfoList(QStringList() << "*.avi",
                                               QDir::Files);
    QDateTime   today      = QDateTime::currentDateTime();
    int         daysToKeep = MySettings().getMaxDays();
    foreach(const QFileInfo& info, infoList)
        if(info.created().daysTo(today) > daysToKeep)
            QFile::remove(info.filePath());
}

/**
 * Remove old videos when storage limit is hit
 */
void SettingsPage::enforceStorageLimit()
{
    qint64 size = 0;
    QDir dir(_settings.getStoragePath());
    QFileInfoList infoList = dir.entryInfoList(QStringList() << "*.avi",
                                               QDir::NoFilter,
                                               QDir::Time | QDir::Reversed);
    // total file size
    foreach(const QFileInfo& info, infoList)
        size += info.size();

    // remove oldest files until size is below limit
    qint64 sizeLimit = MySettings().getMaxStorage() * 1024 * 1024;
    while(!infoList.isEmpty() && size > sizeLimit)
    {
        size -= infoList.front().size();
        QFile(infoList.front().filePath()).remove();
        infoList.removeFirst();
    }
}
