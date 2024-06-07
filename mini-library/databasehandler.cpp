#include "databasehandler.h"

DatabaseHandler::DatabaseHandler(QObject *parent)
    : QObject{parent}
{
    QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName("books.sqlite");
    this->db = db;
}

DatabaseHandler::~DatabaseHandler() {
    this->closeDatabase();
}

void DatabaseHandler::closeDatabase() {
    this->db->close();
}

void DatabaseHandler::initializeDatabase() {
    if (!db->open()) {
        QErrorMessage::qtHandler()->showMessage(db->lastError().text());
        return;
    }

    QSqlQuery query;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS books ( \
                                    id TEXT PRIMARY KEY, \
                                    title TEXT, \
                                    subtitle TEXT, \
                                    authors TEXT, \
                                    isbn TEXT,  \
                                    publisher TEXT, \
                                    publishDate TEXT, \
                                    description TEXT, \
                                    pageCount INTEGER, \
                                    language TEXT, \
                                    thumbnailUrl TEXT, \
                                    link TEXT \
                                );";
    if (!query.exec(createTableQuery)) {
        QErrorMessage::qtHandler()->showMessage(query.lastError().text());
        return;
    }
}

QList<Models::Book> DatabaseHandler::getBooks() {
    QString selectQueryStr = "SELECT * FROM books";
    QSqlQuery selectQuery(selectQueryStr);

    QList<Models::Book> books;

    while (selectQuery.next()) {
        QString id = selectQuery.value("id").toString();
        QString title = selectQuery.value("title").toString();
        QString subtitle = selectQuery.value("subtitle").toString();
        QString authorsStr = selectQuery.value("authors").toString();
        QString isbnStr = selectQuery.value("isbn").toString();
        QString publisher = selectQuery.value("publisher").toString();
        QString publishDate = selectQuery.value("publishDate").toString();
        QString description = selectQuery.value("description").toString();
        int pageCount = selectQuery.value("pageCount").toInt();
        QString language = selectQuery.value("language").toString();
        QString thumbnailUrl = selectQuery.value("thumbnailUrl").toString();
        QString link = selectQuery.value("link").toString();

        // Convert comma-separated strings to QList<QString>
        QList<QString> authors = authorsStr.split(", ");
        QList<QString> isbn = isbnStr.split(", ");

        // Create a new Book object and add it to the list
        Models::Book book(id, title, subtitle, authors, isbn,
                          publisher, publishDate, description,
                          pageCount, language, thumbnailUrl, link);

        books.append(book);
    }
    return books;
}

void DatabaseHandler::saveBook(Models::Book& book) {
    QString existsQuery = R"(SELECT COUNT(*) as count FROM books WHERE id = ?;)";
    QSqlQuery exists;
    exists.prepare(existsQuery);
    exists.addBindValue(book.getId());

    bool useInsert = true;
    if (!exists.exec()) {
        QErrorMessage::qtHandler()->showMessage(exists.lastError().text());
        return;
    }
    if (exists.first()) {
        auto hasCount = exists.value("count").toInt();
        if (hasCount > 0) {
            useInsert = false;
        } else {
            useInsert = true;
        }
    } else {
        useInsert = true;
    }

    QString updateQueryString;
    QSqlQuery updateQuery;
    if (useInsert == true) {
        updateQueryString = R"(INSERT INTO books (
                                                id,
                                                title,
                                                subtitle,
                                                authors,
                                                isbn,
                                                publisher,
                                                publishDate,
                                                description,
                                                pageCount,
                                                language,
                                                thumbnailUrl,
                                                link
                                ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);)";
    } else {
        updateQueryString = R"(UPDATE books
                               SET title = ?,
                               subtitle = ?,
                               authors = ?,
                               isbn = ?,
                               publisher = ?,
                               publishDate = ?,
                               description = ?,
                               pageCount = ?,
                               language = ?,
                               thumbnailUrl = ?,
                               link = ?
                               WHERE id = ?;)";
    }

    updateQuery.prepare(updateQueryString);
    updateQuery.addBindValue(book.getId());
    updateQuery.addBindValue(book.getTitle());
    updateQuery.addBindValue(book.getSubtitle());
    updateQuery.addBindValue(book.getAuthorList());
    updateQuery.addBindValue(book.getIsbnList());
    updateQuery.addBindValue(book.getPublisher());
    updateQuery.addBindValue(book.getPublishDate());
    updateQuery.addBindValue(book.getDescription());
    updateQuery.addBindValue(book.getPageCount());
    updateQuery.addBindValue(book.getLanguage());
    updateQuery.addBindValue(book.getThumbnailUrl());
    updateQuery.addBindValue(book.getLink());

    if (!updateQuery.exec()) {
        QErrorMessage::qtHandler()->showMessage(updateQuery.lastError().text());
        return;
    }
}
