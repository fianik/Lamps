#ifndef SERVERJSON_H
#define SERVERJSON_H

#include <QMainWindow>
#include "paintlamps.h"

#include <QGraphicsScene>

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>

namespace Ui {class ServerJSON;}

class ServerJSON : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerJSON(QWidget *parent = 0);
    ~ServerJSON();

public slots:
    void connectionState(QString);

    void updateItem();

private:
    Ui::ServerJSON *ui;

    QGraphicsScene *scene = new QGraphicsScene;
    PaintLamps *lamp;

    QSqlQueryModel model;
    QGraphicsTextItem *textItem;

    const int figureDistance = 35;

    void paintItem();
};

#endif // SERVERJSON_H
