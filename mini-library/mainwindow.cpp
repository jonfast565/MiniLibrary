#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "utilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scanDialog = new ScanDialog(this);
    this->databaseHandler = new DatabaseHandler(this);
    this->databaseHandler->initializeDatabase();

    this->searchTableModel = new QStandardItemModel(this);
    this->proxyModel = new QSortFilterProxyModel(this);
    this->proxyModel->setSourceModel(this->searchTableModel);

    ui->searchTableView->setModel(this->proxyModel);
    ui->searchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(this->searchTableModel, &QStandardItemModel::dataChanged, this, &MainWindow::onSearchTableItemEdited);
    this->refreshBooks();

    qDebug() << "App path : " << qApp->applicationDirPath();
}

MainWindow::~MainWindow()
{
    delete this->scanDialog;
    delete this->databaseHandler;
    delete ui;
}

void MainWindow::addBookButtonClicked()
{
    this->scanDialog->resetDialog();
    if (this->scanDialog->exec() == QDialog::Accepted) {
        auto currentBook = this->scanDialog->getCurrentBook();
        this->databaseHandler->saveBook(currentBook);
        ui->addBookButton->setFocus();
    }

    this->refreshBooks();
}

void MainWindow::refreshBooks() {
    auto books = this->databaseHandler->getBooks();
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
}

void MainWindow::filterTextChanged(const QString &text) {
    qDebug() << "Search item: " << text;
    proxyModel->setFilterFixedString(text);
    proxyModel->setFilterKeyColumn(1);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void MainWindow::clearSearch() {
    this->ui->searchTextEdit->setText(QString());
}

void MainWindow::onSearchTableItemEdited(const QModelIndex &index) {
    auto selectedRowIndex = index.row();
    auto items = getQStandardItemsForRow(this->searchTableModel, selectedRowIndex);
    auto book = Models::Book::fromQStandardItems(items);
    this->databaseHandler->saveBook(book);
}
