#include "scandialog.h"
#include "ui_scandialog.h"
#include "utilities.h"

ScanDialog::ScanDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ScanDialog) {
    ui->setupUi(this);

    this->searchTableModel = new QStandardItemModel(this);

    ui->searchTable->setModel(this->searchTableModel);
    ui->searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->searchTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchTable->setSelectionMode(QAbstractItemView::SingleSelection);

    this->initDialog();
}

void ScanDialog::resetProgressAndHide() {
    ui->progressBar->setDisabled(true);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
}

void ScanDialog::setProgressIndeterminate() {
    ui->progressBar->setDisabled(false);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->show();
}

void ScanDialog::clearSearchField() {
    ui->searchTextEdit->clear();
    ui->searchTextEdit->setFocus();
}

void ScanDialog::clearSearchResults() {
    if (this->searchTableModel != nullptr && this->searchTableModel->rowCount() > 0) {
        this->searchTableModel->clear();
    }
}

void ScanDialog::clearResultsSection() {
    ui->authorTextEdit->clear();
    ui->authorTextEdit->setDisabled(true);

    ui->titleTextEdit->clear();
    ui->titleTextEdit->setDisabled(true);

    ui->isbnTextEdit->clear();
    ui->isbnTextEdit->setDisabled(true);

    ui->publishDateTextEdit->clear();
    ui->publishDateTextEdit->setDisabled(true);

    ui->publisherTextEdit->clear();
    ui->publisherTextEdit->setDisabled(true);

    ui->pageCountTextEdit->clear();
    ui->pageCountTextEdit->setDisabled(true);

    ui->descriptionTextEdit->clear();
    ui->descriptionTextEdit->setDisabled(true);

    ui->subtitleTextEdit->clear();
    ui->subtitleTextEdit->setDisabled(true);

    ui->languageTextEdit->clear();
    ui->languageTextEdit->setDisabled(true);
}

void ScanDialog::disableSearchControls() {
    ui->searchTable->setDisabled(true);
    ui->searchButton->setDisabled(true);
}

void ScanDialog::enableSearchControls() {
    ui->searchTable->setDisabled(false);
    ui->searchButton->setDisabled(false);
}

void ScanDialog::requestBook() {
    this->setProgressIndeterminate();
    this->disableSearchControls();

    auto isbn = ui->searchTextEdit->text();
    auto searchValue = "isbn:" + isbn;
    auto url = QUrl("https://www.googleapis.com/books/v1/volumes");
    QUrlQuery query;
    query.addQueryItem("q", searchValue);

    // TODO: Add configuration here, this is insecure
    query.addQueryItem("key", "AIzaSyCX3aDxHpJzhVIrJqFR4wpRo_a9uFUNy44");
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            try {
                auto books = Models::Book::fromGoogleBooksApiJson(jsonResponse);
                updateResultsSection(books);
            } catch (std::exception e) {
                QErrorMessage::qtHandler()->showMessage(e.what());
            }
        } else {
            QErrorMessage::qtHandler()->showMessage(
                "Error occurred while handling call to books API");
        }
        reply->deleteLater();
        this->resetProgressAndHide();
        this->enableSearchControls();
    });
}

void ScanDialog::selectFirstRow()
{
    if (this->searchTableModel->rowCount() == 1) {
        QModelIndex firstRowIndex = this->searchTableModel->index(0, 0);
        ui->searchTable->selectionModel()->select(firstRowIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);

        ui->searchTable->selectionModel()->select(firstRowIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        ui->searchTable->setCurrentIndex(firstRowIndex);
        ui->searchTable->scrollTo(firstRowIndex);

        emit ui->searchTable->clicked(firstRowIndex);
    }
}

void ScanDialog::updateResultsSection(QList<Models::Book> &books) {
    this->searchTableModel->clear();

    this->searchTableModel->setColumnCount(12);
    this->searchTableModel->setHorizontalHeaderLabels(QStringList()
                                                      << "Id"
                                                      << "Title"
                                                      << "Subtitle"
                                                      << "Authors"
                                                      << "ISBNs"
                                                      << "Publisher"
                                                      << "Publish Date"
                                                      << "Description"
                                                      << "Page Count"
                                                      << "Language"
                                                      << "Thumbnail"
                                                      << "Link");

    for (Models::Book &book : books) {
        auto bookItems = book.toQStandardItems();
        this->searchTableModel->appendRow(bookItems);
    }

    selectFirstRow();
}

void ScanDialog::setResults() {
    auto selectedRows = ui->searchTable->selectionModel()->selectedRows();
    if (selectedRows.length() <= 0) {
        return;
    }
    auto selectedRowIndex = selectedRows.at(0).row();
    auto items = getQStandardItemsForRow(this->searchTableModel, selectedRowIndex);
    auto book = Models::Book::fromQStandardItems(items);
    this->setScanValues(book);
    this->setCurrentBook(book);
}

void ScanDialog::setScanValues(Models::Book& book) {
    ui->authorTextEdit->setText(book.getAuthorList());
    ui->titleTextEdit->setText(book.getTitle());
    ui->subtitleTextEdit->setText(book.getSubtitle());
    ui->descriptionTextEdit->setText(book.getDescription());
    ui->isbnTextEdit->setText(book.getIsbnList());
    ui->publisherTextEdit->setText(book.getPublisher());
    ui->publishDateTextEdit->setText(book.getPublishDate());
    ui->pageCountTextEdit->setText(QString::number(book.getPageCount()));
    ui->languageTextEdit->setText(book.getLanguage());
}

void ScanDialog::setCurrentBook(Models::Book& book) {
    this->currentBook = book;
}

Models::Book ScanDialog::getCurrentBook() {
    return this->currentBook;
}

void ScanDialog::searchRequestAuto(const QString& searchTerm) {
    if (isValidISBN(searchTerm)) {
        this->ui->searchButton->click();
    }
}

void ScanDialog::initDialog() {
    clearResultsSection();
    clearSearchField();
    resetProgressAndHide();
    clearSearchResults();
}

void ScanDialog::resetDialog() {
    this->initDialog();
}

ScanDialog::~ScanDialog() {
    delete ui;
    delete this->searchTableModel;
}
