#include "serverjson.h"
#include "ui_serverjson.h"

ServerJSON::ServerJSON(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerJSON)
{
    ui->setupUi(this);
    paintItem();
}

void ServerJSON::paintItem()
{
    scene->clear();
    ui->graphicsView_Lamps->setScene(scene);
    model.setQuery("SELECT * FROM listlamps");

    for (int i = 0; i < model.rowCount(); ++i)
    {
        int id = model.record(i).value("id").toInt();
        int state = model.record(i).value("state").toInt();
        qDebug() << "ID:" << id << "State:"  << state;

        lamp = new PaintLamps();
        scene->addItem(lamp);
        if (state == 1){lamp->setBrush(Qt::green);}
        else if (state == 0){lamp->setBrush(Qt::red);}
        lamp->init_item(0,figureDistance*i);

        QString text = "Лампочка №" + QString::number(id);
        textItem = this->scene->addText(text);
        textItem->setPos(-60,figureDistance*i-15);
    }
}

ServerJSON::~ServerJSON()
{
    delete ui;
}

void ServerJSON::connectionState(QString text)
{
    ui->textBrowser_ConnectionStatus->append(text);
}

void ServerJSON::updateItem()
{
    qDebug() << "STATUS: Change item";
    paintItem();
}
