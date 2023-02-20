#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>

#include <QSqlDatabase>
#include <QSqlQuery>

#include "serverjson.h"

class Server: public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

    QTcpSocket* socket;
    int id;
    int state;

private:
    QByteArray Data;

    QJsonDocument docJson;
    QJsonParseError docJsonError;

    QSqlDatabase db;
    QSqlQuery *query;

    QByteArray output;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

signals:
    void resAvailable(QString text);

    void itemUpdate();
};
#endif // SERVER_H
