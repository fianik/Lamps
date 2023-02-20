#include "server.h"

Server::Server()
{
    socket = new QTcpSocket(this);
    if(this->listen(QHostAddress::Any, 4007))
    {
        qDebug() << "STATUS: Server started";
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("./../../db/lamps.db");
        if (db.open())
        {
            qDebug() << "STATUS: DB is open";
        }
        else
        {
            qDebug() << "ERROR: DB is not open";
        }
    }
    else
    {
        qDebug() << "ERROR: Server did not start";
        socket->close();
        throw socket->errorString();
    }
}

Server::~Server(){}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket->setSocketDescriptor(socketDescriptor);

    connect (socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect (socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    qDebug() << "STATUS: Client" << socketDescriptor << "connected";
    socket->write("{\"type\":\"connect\",\"status\":\"yes\"}");

    QString resConnect = "Соединение установлено";
    emit resAvailable(resConnect);                                                      // Отправить статус соединения на форму сервера
}

void Server::slotReadyRead()
{
    Data = socket->readAll();
    qDebug () << "Data:" << Data;

    docJson = QJsonDocument::fromJson(Data,&docJsonError);
    if (docJsonError.errorString() == "no error occurred")
    {
        if((docJson.object().value("type").toString() == "recSize") && (docJson.object().value("resp").toString() == "lamps"))
        {
            output = "{\"type\":\"resultSelect\",\"result\":[";
            if (db.isOpen())
            {
                query = new QSqlQuery(db);
                if (query->exec("SELECT id FROM listlamps"))
                {
                    while (query->next())
                    {
                        output.append("{\"id\":\"" + query->value(0).toString() + "\"},");
                    }
                    output.remove(output.length()-1,1);
                }
                else
                {
                    qDebug() << "ERROR: Query is not success";
                }
                delete query;
            }
            output.append("]}");
            
            socket->write("{\"type\":\"size\",\"resp\":\"lamps\",\"length\":" + QByteArray::number(output.size()) + "}");
            socket->waitForBytesWritten(500);
        }

        else if((docJson.object().value("type").toString() == "select") && (docJson.object().value("params").toString() == "lamps"))
        {
            socket->write(output);
            qDebug() << "Message size:" << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }

        // Оброботка JSON команд с кнопок клиента
        else if (docJson.object().value("command").toString() == "add")
        {
            if (db.isOpen())
            {
                query = new QSqlQuery(db);
                if (query->exec("INSERT INTO listlamps (id, state) "
                                "VALUES ((SELECT MAX(id) FROM listlamps) + 1, '0')"))
                {
                    id = query->lastInsertId().toInt() + 1;
                    socket->write("{\"type\":\"resultSelect\",\"status\":\"add\",\"id\":\"" + QByteArray::number(id) + "\"}");
                    emit itemUpdate();
                }
                else
                {
                    qDebug() << "ERROR: Query is not success";
                }
                delete query;
            }
            socket->waitForBytesWritten(500);
        }

        else if (docJson.object().value("command").toString() == "delete")
        {
            if (db.isOpen())
            {
                query = new QSqlQuery(db);
                if (query->exec("DELETE FROM listlamps "
                                "WHERE id = '"+ docJson.object().value("id").toString() +"'"))
                {
                    id = docJson.object().value("id").toInt();
                    emit itemUpdate();
                }
                else
                {
                    qDebug() << "ERROR: Query is not success";
                }
                delete query;
            }
        }

        else if (docJson.object().value("command").toString() == "changestatus")
        {
            if (db.isOpen())
            {
                query = new QSqlQuery(db);
                if (docJson.object().value("status").toString() == "on")
                {
                    if (query->exec("UPDATE listlamps "
                                    "SET state = 1 "
                                    "WHERE id = '"+ docJson.object().value("id").toString() +"'"))
                    {
                        id = docJson.object().value("id").toInt();
                        state = docJson.object().value("state").toInt();
                        emit itemUpdate();
                    }
                    else
                    {

                        qDebug() << "ERROR: Query is not success";
                    }
                }
                else if(docJson.object().value("status").toString() == "off")
                {
                    if (query->exec("UPDATE listlamps "
                                    "SET state = 0 "
                                    "WHERE id = '"+ docJson.object().value("id").toString() +"'"))
                    {
                        id = docJson.object().value("id").toInt();
                        state = docJson.object().value("state").toInt();
                        emit itemUpdate();
                    }
                    else
                    {
                        qDebug() << "ERROR: Query is not success";
                    }
                }
                delete query;
            }
        }

    }
}
