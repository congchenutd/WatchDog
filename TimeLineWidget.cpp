#include "TimeLineWidget.h"

#include <QPainter>
#include <QMouseEvent>

TimeLineWidget::TimeLineWidget(QWidget* parent)
    : QWidget(parent), _margins(10, 50, 10, 10)
{
    _tickGap = 1;
    _mouseX  = -1;
}

void TimeLineWidget::setRange(const QDateTime& start, const QDateTime& end)
{
    _start  = start;
    _end    = end;
    _mouseX = -1;
    update();
}

void TimeLineWidget::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    update();
}

void TimeLineWidget::setMargins(int left, int top, int right, int bottom)
{
    _margins = QMargins(left, top, right, bottom);
    update();
}

void TimeLineWidget::addEventSerie(int id, const QColor& color) {
    _series.insert(id, EventSerie(id, color));
}

void TimeLineWidget::addEvent(int serieID, const QDateTime& start, const QDateTime& end)
{
    if (_series.contains(serieID))
    {
        _series[serieID].addEvent(TimeLineEvent(start, end));
        update();
    }
}

QDateTime TimeLineWidget::getStart() const { return _start; }
QDateTime TimeLineWidget::getEnd  () const { return _end;   }

void TimeLineWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), _backgroundColor);

    painter.setPen(Qt::black);
    painter.drawRect(contentRect());

    const int hours = _start.secsTo(_end) / 3600;
    if (hours == 0)
        return;

    const int tickWidth = contentRect().width() / hours;
    for (int i = 0; i <= hours; i += _tickGap)
    {
        // tick
        const int x = _margins.left() + tickWidth * i;
        painter.drawLine(x, _margins.top(), x, _margins.top() - TickLength);

        // hour label
        QDateTime time = _start.addSecs(3600*i);
        const int hour = time.time().hour();
        painter.drawText(x, _margins.top() - LabelSpace, tr("%1").arg(hour));

        // date label
        if (hour == 0)
            painter.drawText(x, _margins.top() - DateSpace, tr("%1").arg(time.toString("MMM-dd")));
    }

    // events
    foreach(const EventSerie& serie, _series)
        foreach(const TimeLineEvent& event, serie._events)
        {
            int startX = timeToX(event._start);
            int endX   = timeToX(event._end);
            painter.fillRect(startX, contentRect().top(), endX - startX, contentRect().height(), serie._color);
        }

    // mouse click line
    if (_mouseX > -1)
    {
        painter.setPen(QPen(Qt::red));
        painter.drawLine(_mouseX, rect().top(), _mouseX, rect().bottom());
    }
}

/**
 * Adjust tick gap based on the width of the widget
 * Ensure the gap is larger than MinTickGap
 */
void TimeLineWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);

    // start from smallest value
    _tickGap = 1;
    int nTicks = _start.secsTo(_end) / 3600 / _tickGap;
    if (nTicks <= 0)
        return;

    // increase gap until tick width is large enough
    int tickWidth = contentRect().width() / nTicks;
    while (tickWidth < MinTickWidth)
    {
        _tickGap ++;
        nTicks = _start.secsTo(_end) / 3600 / _tickGap;
        tickWidth = contentRect().width() / nTicks;
    }
}

void TimeLineWidget::mousePressEvent(QMouseEvent* e)
{
    _mouseX = e->pos().x();
    emit mouseClicked(xToTime(_mouseX));
    update();
}

QRect TimeLineWidget::contentRect() const {
    return rect().marginsRemoved(_margins);
}

/**
 * Convert time to distance to the left content rect border in pixel
 */
int TimeLineWidget::timeToX(const QDateTime& time) const
{
    if (time < _start || time > _end)
        return -1;

    return (double) _start.msecsTo(time) / _start.msecsTo(_end) * contentRect().width() + _margins.left();
}

/**
 * Convert distance to the left content rect border in pixel to time
 */
QDateTime TimeLineWidget::xToTime(int x) const
{
    qint64 seconds = (double) (x - _margins.left()) / contentRect().width() * _start.msecsTo(_end);
    return _start.addMSecs(seconds);
}

