#ifndef REVIEWPAGE_H
#define REVIEWPAGE_H

#include "ui_ReviewPage.h"

class ReviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReviewPage(QWidget *parent = 0);

private:
    Ui::ReviewPage ui;
};

#endif // REVIEWPAGE_H
