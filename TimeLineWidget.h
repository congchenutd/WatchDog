#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QDateTime>
#include <QWidget>
#include <QColor>
#include <QMap>

struct TimeLineEvent
{
    QDateTime _start;
    QDateTime _end;

    TimeLineEvent(const QDateTime& start, const QDateTime& end)
        : _start(start), _end(end) {}
};

struct EventSerie
{
    int     _id;
    QColor  _color;
    QList<TimeLineEvent> _events;

    EventSerie() {}
    EventSerie(int id, const QColor& color) : _id(id), _color(color) {}
    void addEvent(const TimeLineEvent& event) { _events << event; }
};

class TimeLineWidget: public QWidget
{
    Q_OBJECT

public:
    TimeLineWidget(QWidget* parent = 0);
    void setRange(const QDateTime& start, const QDateTime& end);
    void setBackgroundColor(const QColor& color);
    void setMargins(int left, int top, int right, int bottom);
    void addEventSerie(int id, const QColor& color);
    void addEvent(int serieID, const QDateTime& start, const QDateTime& end);

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent* e);

private:
    int contentWidth() const;
    QRect contentRect() const;
    int timeToX(const QDateTime& time) const;
    QDateTime xToTime(int x) const;

signals:
    void mouseClicked(const QDateTime& time);

private:
    QDateTime   _start;
    QDateTime   _end;
    QColor      _backgroundColor;
    QMargins    _margins;
    const static int TickLength = 5;
    const static int LabelSpace = 10;
    const static int DateSpace  = 30;
    const static int MinTickWidth = 20;
    int _tickGap;
    int _mouseX;

    QMap<int, EventSerie> _series;
};

#endif // TIMELINEWIDGET_H
