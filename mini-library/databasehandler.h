#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QStandardItem>
#include <QList>
#include <QDebug>
#include <QErrorMessage>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include "models.h"

class DatabaseHandler : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase *db;

public:
    explicit DatabaseHandler(QObject *parent = nullptr);
    void initializeDatabase();
    QList<Models::Book> getBooks();
    void saveBook(Models::Book& book);

signals:
};

#endif // DATABASEHANDLER_H
