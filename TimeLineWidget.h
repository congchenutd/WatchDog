#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QDateTime>
#include <QWidget>
#include <QColor>
#include <QMap>

/**
 * A period on the time line
 */
struct TimeLineEvent
{
    QDateTime _start;
    QDateTime _end;

    TimeLineEvent(const QDateTime& start, const QDateTime& end)
        : _start(start), _end(end) {}
};

/**
 * A serie of events have the same color
 */
struct EventSerie
{
    int     _id;
    QColor  _color;
    QList<TimeLineEvent> _events;

    EventSerie() {}
    EventSerie(int id, const QColor& color) : _id(id), _color(color) {}
    void addEvent(const TimeLineEvent& event) { _events << event; }
};

/**
 * A widget showing a time line
 */
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
    QDateTime getStart() const;
    QDateTime getEnd  () const;

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent* e);

private:
    QRect contentRect() const;
    int timeToX(const QDateTime& time) const;
    QDateTime xToTime(int x) const;

signals:
    void mouseClicked(const QDateTime& time);

private:
    const static int TickLength     = 5;
    const static int LabelSpace     = 10;   // space between label and content rect
    const static int DateSpace      = 30;   // space between date and content rect
    const static int MinTickWidth   = 20;   // min width (in pixel) between ticks
    QDateTime   _start;
    QDateTime   _end;
    QColor      _backgroundColor;
    QMargins    _margins;               // margins of the content rect
    int _tickGap;                       // hour gap between two ticks, dynamically adjusted
    int _mouseX;                        // x coordinates of a mouse click

    QMap<int, EventSerie> _series;
};

#endif // TIMELINEWIDGET_H
