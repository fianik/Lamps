#include "paintlamps.h"

PaintLamps::PaintLamps()
{
    setPen(QPen(Qt::black));
    setBrush(Qt::black);
}

PaintLamps::~PaintLamps(){}

void PaintLamps::init_item(uint16_t item_x,uint16_t item_y)
{
    x = item_x;
    y = item_y;
    this->setPos(item_x, item_y);
}

QRectF PaintLamps::boundingRect() const
{
    return QRectF(-100,-15,175,25);
}

void PaintLamps::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(-100,-15,175,25);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
