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
}

void TimeLineWidget::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    update();
}

void TimeLineWidget::setMargins(int left, int top, int right, int bottom) {
    _margins = QMargins(left, top, right, bottom);
}

void TimeLineWidget::addEventSerie(int id, const QColor& color) {
    _series.insert(id, EventSerie(id, color));
}

void TimeLineWidget::addEvent(int serieID, const QDateTime& start, const QDateTime& end)
{
    if (_series.contains(serieID))
        _series[serieID].addEvent(TimeLineEvent(start, end));
}

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
        const int x = _margins.left() + tickWidth * i;
        painter.drawLine(x, _margins.top(), x, _margins.top() - TickLength);

        QDateTime time = _start.addSecs(3600*i);
        const int hour = time.time().hour();
        painter.drawText(x, _margins.top() - LabelSpace, tr("%1").arg(hour));

        if (hour == 0)
            painter.drawText(x, _margins.top() - DateSpace, tr("%1").arg(time.toString("MMM-dd")));
    }

    foreach(const EventSerie& serie, _series)
    {
        foreach(const TimeLineEvent& event, serie._events)
        {
            int startX = timeToX(event._start);
            int endX   = timeToX(event._end);
            painter.fillRect(startX, contentRect().top(), endX - startX, contentRect().height(), serie._color);
        }
    }

    if (_mouseX > -1)
    {
        painter.setPen(QPen(Qt::red));
        painter.drawLine(_mouseX, contentRect().top(), _mouseX, contentRect().bottom());
    }
}

void TimeLineWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    _tickGap = 1;
    int nTicks = _start.secsTo(_end) / 3600 / _tickGap;
    if (nTicks <= 0)
        return;
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
    update();
    emit mouseClicked(xToTime(_mouseX));
    QWidget::mousePressEvent(e);
}

QRect TimeLineWidget::contentRect() const {
    return rect().marginsRemoved(_margins);
}

int TimeLineWidget::timeToX(const QDateTime& time) const
{
    if (time < _start || time > _end)
        return -1;

    return (double) _start.secsTo(time) / _start.secsTo(_end) * contentRect().width() + _margins.left();
}

QDateTime TimeLineWidget::xToTime(int x) const
{
    qint64 seconds = (double) (x - _margins.left()) / contentRect().width() * _start.secsTo(_end);
    return _start.addSecs(seconds);
}

