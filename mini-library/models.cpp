#include "models.h"

Models::Book::Book(QObject *parent)
    : QObject{parent}
{}

QList<Models::Book> Models::Book::fromGoogleBooksApiJson(QJsonDocument &document)
{
    if (!document.isObject()) {
        throw std::exception("Document root is not object");
    }
    auto root = document.object();

    if (!root.contains("items")) {
        throw std::exception("Root doesn't contain items");
    }

    auto items = root["items"];
    if (!items.isArray()) {
        throw std::exception("Items not array");
    }
    auto itemsArray = items.toArray();

    QList<Models::Book> booksList;
    for (auto item : itemsArray) {
        QList<QString> isbnList;
        QList<QString> authorList;

        if (!item.isObject()) {
            throw std::exception("Item is not object");
        }

        auto itemObject = item.toObject();

        auto volumeInfo = itemObject["volumeInfo"];
        if (!volumeInfo.isObject()) {
            throw std::exception("Volume info is not an object");
        }
        auto volumeInfoObject = volumeInfo.toObject();

        auto accessInfo = itemObject["accessInfo"];
        if (!accessInfo.isObject()) {
            throw std::exception("Access info is not an object");
        }
        auto accessInfoObject = accessInfo.toObject();

        auto industryIdentifiers = volumeInfoObject["industryIdentifiers"];
        if (!industryIdentifiers.isArray()) {
            throw std::exception("Industry identifiers is not an object");
        }
        auto industryIdentifiersObject = industryIdentifiers.toArray();
        for (const QJsonValue& industryIdentifier: industryIdentifiersObject) {
            auto identifier = industryIdentifier["identifier"].toString();
            isbnList.append(identifier);
        }

        auto authors = volumeInfoObject["authors"];
        if (!authors.isArray()) {
            throw std::exception("Authors is not an array");
        }
        auto authorsObject = authors.toArray();
        for (const QJsonValue& author: authorsObject) {
            authorList.append(author.toString());
        }

        auto id = itemObject["id"].toString();
        auto title = volumeInfoObject["title"].toString();
        auto subtitle = volumeInfoObject["subtitle"].toString();
        auto publisher = volumeInfoObject["publisher"].toString();
        auto publishDate = volumeInfoObject["publishedDate"].toString();
        auto description = volumeInfoObject["description"].toString();
        auto pageCount = volumeInfoObject["pageCount"].toInt();
        auto language = volumeInfoObject["language"].toString();
        auto imageLinks = volumeInfoObject["imageLinks"].toObject();
        auto thumbnailUrl = imageLinks["thumbnail"].toString();
        auto link = accessInfoObject["webReaderLink"].toString();
        auto book = Book(id, title, subtitle, authorList, isbnList, publisher, publishDate, description, pageCount, language, thumbnailUrl, link);

        booksList.append(book);
    }

    return booksList;
}

QList<QStandardItem*> Models::Book::toQStandardItems() {
    QList<QStandardItem*> items;

    items.append(new QStandardItem(this->id));
    items.append(new QStandardItem(this->title));
    items.append(new QStandardItem(this->subtitle));
    items.append(new QStandardItem(this->getAuthorList()));
    items.append(new QStandardItem(this->getIsbnList()));
    items.append(new QStandardItem(this->publisher));
    items.append(new QStandardItem(this->publishDate));
    items.append(new QStandardItem(this->description));
    items.append(new QStandardItem(QString::number(this->pageCount)));
    items.append(new QStandardItem(this->language));
    items.append(new QStandardItem(this->thumbnailUrl));
    items.append(new QStandardItem(this->link));

    return QList<QStandardItem*>(items);
}

Models::Book Models::Book::fromQStandardItems(const QList<QStandardItem*>& items) {
    if (items.size() != 12) {
        throw std::invalid_argument("Invalid number of QStandardItems");
    }

    QString id = items[0]->text();
    QString title = items[1]->text();
    QString subtitle = items[2]->text();
    QList<QString> authors = items[3]->text().split(", ");
    QList<QString> isbn = items[4]->text().split(", ");
    QString publisher = items[5]->text();
    QString publishDate = items[6]->text();
    QString description = items[7]->text();
    int pageCount = items[8]->text().toInt();
    QString language = items[9]->text();
    QString thumbnailUrl = items[10]->text();
    QString link = items[11]->text();

    return Models::Book(id,
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
                        link);
}

QString Models::Book::getAuthorList() {
    QString result;
    for (const QString& author: this->authors) {
        result += author + ", ";
    }
    result = result.removeLast();
    result = result.removeLast();
    return QString(result);
}

QString Models::Book::getIsbnList() {
    QString result;
    for (const QString& isbn: this->isbn) {
        result += isbn + ", ";
    }
    result = result.removeLast();
    result = result.removeLast();
    return QString(result);
}
