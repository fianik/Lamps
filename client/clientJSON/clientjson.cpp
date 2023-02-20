#include "clientjson.h"
#include "ui_clientjson.h"

ClientJSON::ClientJSON(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientJSON)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead, this, &ClientJSON::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

ClientJSON::~ClientJSON()
{
    delete ui;
}

void ClientJSON::on_pushbutton_Connect_clicked()
{
    QString addr = ui->lineEdit_EnterIP->text();
    QString portStr = ui->lineEdit_EnterPort->text();
    int port = portStr.toInt();

    if ((addr == "127.0.0.1")&&(port == 4007))
    {
        ui->textBrowser_ConnectionStatus->clear();
        socket->connectToHost(addr, port);
    }
    else
    {
        ui->textBrowser_ConnectionStatus->clear();
        ui->textBrowser_ConnectionStatus->append("Соединение невозможно");
    }
}

void ClientJSON::slotReadyRead()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        
        if(!complexData)
        {
            Data = socket->readAll();            
        }
        else
        {
            Data.append(socket->readAll());
            complexData = false;
        }
        
        docJson = QJsonDocument::fromJson(Data,&docJsonError);
        if (docJsonError.errorString() == "no error occurred")
        {
            model = new QStandardItemModel(nullptr);

            if ((docJson.object().value("type").toString() == "connect") && (docJson.object().value("status").toString() == "yes"))
            {
                if (socket->isOpen())
                {
                    ui->textBrowser_ConnectionStatus->append("Соединение установлено");
                    socket->write("{\"type\":\"recSize\",\"resp\":\"lamps\"}");
                    socket->waitForBytesWritten(500);
                }
                else
                {
                    QMessageBox::information(this, "Статус","Соединение не установлено");
                }
            }
            else if (docJson.object().value("type").toString() == "resultSelect")
            {
                qDebug() << "Получено:" << Data.size() << ".Необходимо: " << reqSize;
                if (Data.size() == reqSize)
                {
                    model->setHorizontalHeaderLabels(QStringList() << "id");
                    QJsonArray docJsonArr = docJson.object().value("result").toArray();
                    for (int i = 0; i < docJsonArr.count(); i++)
                    {
                        col = new QStandardItem("Лампочка №" + docJsonArr[i].toObject().value("id").toString());
                        model->appendRow(col);
                    }
                    ui->comboBox_Lamps->setModel(model);
                }
                else if (docJson.object().value("status").toString() == "add")
                {
                    addNum = "Лампочка №" + docJson.object().value("id").toString();
                    ui->comboBox_Lamps->addItem(addNum);
                }
            }
            else if((docJson.object().value("type").toString() == "size") && (docJson.object().value("resp").toString() == "lamps"))
            {
                reqSize = docJson.object().value("length").toInt();
                socket->write("{\"type\":\"select\",\"params\":\"lamps\"}");
            }
            else
            {
                complexData = true;
                slotReadyRead();
            }
        }
        else
        {
            QMessageBox::information(this, "Статус", "Ошибка формата передачи данных: " + docJsonError.errorString());
        }
    }
}

void ClientJSON::on_pushButton_AddItem_clicked()
{
    if (socket->isOpen())
    {
        socket->write("{\"command\":\"add\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this, "Статус","Соединение не установлено");
    }
}

QString ClientJSON::isolateNumFromText()
{
    QString dataBox = ui->comboBox_Lamps->currentText();
    unsigned startIndex = dataBox.indexOf(QRegExp("[0-9]"));
    unsigned finishIndex = dataBox.lastIndexOf(QRegExp("[0-9]"));
    return dataBox.mid(startIndex, finishIndex - startIndex + 1);
}

void ClientJSON::on_pushButton_DeleteItem_clicked()
{
    if (socket->isOpen())
    {
        QString numLamp = isolateNumFromText();
        socket->write("{\"command\":\"delete\",\"id\":\"" + numLamp.toUtf8() + "\"}");
        socket->waitForBytesWritten(500);
        ui->comboBox_Lamps->removeItem(ui->comboBox_Lamps->currentIndex());
    }
    else
    {
        QMessageBox::information(this, "Статус","Соединение не установлено");
    }
}

void ClientJSON::on_pushButton_LampOn_clicked()
{
    if (socket->isOpen())
    {
        QString numLamp = isolateNumFromText();
        socket->write("{\"command\":\"changestatus\",\"status\":\"on\",\"id\":\"" + numLamp.toUtf8() + "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this, "Статус","Соединение не установлено");
    }
}

void ClientJSON::on_pushButton_LampOff_clicked()
{
    if (socket->isOpen())
    {
        QString numLamp = isolateNumFromText();
        socket->write("{\"command\":\"changestatus\",\"status\":\"off\",\"id\":\"" + numLamp.toUtf8() + "\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this, "Статус","Соединение не установлено");
    }
}
