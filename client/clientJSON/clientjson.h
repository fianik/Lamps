#ifndef CLIENTJSON_H
#define CLIENTJSON_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <QStandardItem>
#include <QJsonArray>

namespace Ui {class ClientJSON;}

class ClientJSON : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientJSON(QWidget *parent = 0);
    ~ClientJSON();

private:
    Ui::ClientJSON *ui;

    QTcpSocket* socket;
    QByteArray Data;

    QStandardItemModel* model;
    QStandardItem* col;

    QString addNum;
    
    QJsonDocument docJson;
    QJsonParseError docJsonError;

    int reqSize;
    bool complexData = false;

    QString isolateNumFromText();

public slots:
    void slotReadyRead();

private slots:
    void on_pushbutton_Connect_clicked();
    void on_pushButton_AddItem_clicked();
    void on_pushButton_DeleteItem_clicked();
    void on_pushButton_LampOn_clicked();
    void on_pushButton_LampOff_clicked();
};

#endif // CLIENTJSON_H
