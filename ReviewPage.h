#ifndef REVIEWPAGE_H
#define REVIEWPAGE_H

#include "ui_ReviewPage.h"

class ReviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReviewPage(QWidget* parent = 0);

private slots:
    void onToday();
    void onPrevHour();
    void onNextHour();
    void onPrevDay();
    void onNextDay();
    void onRangeChanged();
    void onPlayVideo(const QDateTime& time);
    void updateTimeLine();

private:
    void moveRangeBy(int hours);

private:
    Ui::ReviewPage ui;
};

#endif // REVIEWPAGE_H
