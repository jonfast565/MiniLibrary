#ifndef MODELS_H
#define MODELS_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QStandardItem>

QT_BEGIN_NAMESPACE
namespace Models {
class Book;
} // namespace Models
QT_END_NAMESPACE

// class
class Models::Book : QObject {
    Q_OBJECT
private:
    QString id;
    QString title;
    QString subtitle;
    QList<QString> authors;
    QList<QString> isbn;
    QString publisher;
    QString publishDate;
    QString description;
    int pageCount;
    QString language;
    QString thumbnailUrl;
    QString link;

public:
    explicit Book(QObject *parent = nullptr);

    Book(QString id, QString title, QString subtitle, QList<QString> authors,
         QList<QString> isbn, QString publisher, QString publishDate,
         QString description, int pageCount, QString language,
         QString thumbnailUrl, QString link)
        : id(id), title(title), subtitle(subtitle), authors(authors), isbn(isbn),
        publisher(publisher), publishDate(publishDate),
        description(description), pageCount(pageCount), language(language),
        thumbnailUrl(thumbnailUrl), link(link) {}

    Book(const Models::Book &book)
        : QObject(book.parent()), // Copy the parent QObject
        id(book.id), title(book.title), subtitle(book.subtitle),
        authors(book.authors), isbn(book.isbn), publisher(book.publisher),
        publishDate(book.publishDate), description(book.description),
        pageCount(book.pageCount), language(book.language),
        thumbnailUrl(book.thumbnailUrl), link(book.link) {}

    Models::Book &operator=(const Models::Book &book) {
        if (this == &book) {
            return *this;
        }

        setParent(book.parent());

        id = book.id;
        title = book.title;
        subtitle = book.subtitle;
        authors = book.authors;
        isbn = book.isbn;
        publisher = book.publisher;
        publishDate = book.publishDate;
        description = book.description;
        pageCount = book.pageCount;
        language = book.language;
        thumbnailUrl = book.thumbnailUrl;
        link = book.link;

        return *this;
    }

    QList<QStandardItem *> toQStandardItems();
    QString getAuthorList();
    QString getIsbnList();
    QString getId() const { return id; }
    QString getTitle() const { return title; }
    QString getSubtitle() const { return subtitle; }
    QList<QString> getAuthors() const { return authors; }
    QList<QString> getIsbn() const { return isbn; }
    QString getPublisher() const { return publisher; }
    QString getPublishDate() const { return publishDate; }
    QString getDescription() const { return description; }
    int getPageCount() const { return pageCount; }
    QString getLanguage() const { return language; }
    QString getThumbnailUrl() const { return thumbnailUrl; }
    QString getLink() const { return link; }

    static Book fromQStandardItems(const QList<QStandardItem *> &items);
    static QList<Book> fromGoogleBooksApiJson(QJsonDocument &document);
};

#endif // MODELS_H
