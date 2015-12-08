#include "SettingsPage.h"
#include <QDateTime>
#include <QFileDialog>

SettingsPage::SettingsPage(QWidget* parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.btSave,          SIGNAL(clicked()), this, SLOT(onSave()));
    connect(ui.btStoragePath,   SIGNAL(clicked()), this, SLOT(onSetStoragePath()));
    enforceTimeLimit();
    enforceStorageLimit();
}

void SettingsPage::load()
{
    ui.leUrl            ->setText   (_settings.getUrl());
    ui.sbFPS            ->setValue  (_settings.getFPS());
    ui.sbWidth          ->setValue  (_settings.getMinWidth());
    ui.sbHeight         ->setValue  (_settings.getMinHeight());
    ui.sbNumber         ->setValue  (_settings.getMinNumber());
    ui.sbStorageInterval->setValue  (_settings.getStorageInterval());
    ui.leStoragePath    ->setText   (_settings.getStoragePath());
    ui.sbMaxStorage     ->setValue  (_settings.getMaxStorage());
    ui.sbMaxDays        ->setValue  (_settings.getMaxDays());
}

void SettingsPage::onSave()
{
    _settings.setUrl            (ui.leUrl->text());
    _settings.setFPS            (ui.sbFPS->value());
    _settings.setMinWidth       (ui.sbWidth ->value());
    _settings.setMinHeight      (ui.sbHeight->value());
    _settings.setMinNumber      (ui.sbNumber->value());
    _settings.setStorageInterval(ui.sbStorageInterval->value());
    _settings.setStoragePath    (ui.leStoragePath->text());
    _settings.setMaxStorage     (ui.sbMaxStorage->value());
    _settings.setMaxDays        (ui.sbMaxDays->value());
}

void SettingsPage::onSetStoragePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ".",  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
        ui.leStoragePath->setText(dir);
}

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

void SettingsPage::enforceStorageLimit()
{
    qint64 size = 0;
    QDir dir(_settings.getStoragePath());
    QFileInfoList infoList = dir.entryInfoList(QStringList() << "*.avi",
                                               QDir::NoFilter,
                                               QDir::Time | QDir::Reversed);
    foreach(const QFileInfo& info, infoList)
        size += info.size();
    while(!infoList.isEmpty() && size > 10 * 1024 * 1024)
    {
        size -= infoList.front().size();
        QFile(infoList.front().filePath()).remove();
        infoList.removeFirst();
    }
}
