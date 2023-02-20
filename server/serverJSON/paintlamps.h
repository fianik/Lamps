#ifndef PAINTLAMPS_H
#define PAINTLAMPS_H

#include <QGraphicsRectItem>
#include <QGraphicsView>

class PaintLamps : public QAbstractGraphicsShapeItem
{
public:
     PaintLamps();
    ~PaintLamps();

    void init_item(uint16_t item_x, uint16_t item_y);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const override;

private:
    uint16_t x = 0, y = 0;
};

#endif // PAINTLAMPS_H
