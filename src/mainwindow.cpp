#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupInternal();
}

MainWindow::~MainWindow()
{
    delete ui;
    db->close();
    delete db;
}

void MainWindow::setupInternal()
{
    QSettings settings(QString("config.ini"), QSettings::IniFormat);
    QString server = settings.value("server", "localhost").toString();
    QString database = settings.value("database", "minilibrary").toString();
    int port = settings.value("port", "5433").toInt();
    QString username = settings.value("username", "postgres").toString();
    QString password = settings.value("password", "defaultpass").toString();

    setupDatabaseConnection(server, port, database, username, password);
}

void MainWindow::setupDatabaseConnection(QString server, int port, QString database, QString username, QString password)
{
    QSqlDatabase db = QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));

    db.setHostName(server);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPort(port);
    db.setPassword(password);

    bool ok = db.open();
    if (!ok) {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", db.lastError().text());
        messageBox.setFixedSize(800, 400);
        messageBox.show();
        exit(-1);
    }

    qDebug("Connection established.");
    this->db = &db;

    QSqlQueryModel  *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(*this->db);

    query->prepare("select title, notes, number_of_pages, publish_date, url from books;");
    query->exec();

    model->setQuery(*query);
    model->setHeaderData(0, Qt::Orientation::Horizontal, "Title", Qt::EditRole);
    model->setHeaderData(1, Qt::Orientation::Horizontal, "Notes", Qt::EditRole);
    model->setHeaderData(2, Qt::Orientation::Horizontal, "Number of Pages", Qt::EditRole);
    model->setHeaderData(3, Qt::Orientation::Horizontal, "Publish Date", Qt::EditRole);
    model->setHeaderData(4, Qt::Orientation::Horizontal, "Url", Qt::EditRole);
    ui->tableView->setModel(model);
}

void MainWindow::on_scanButton_clicked()
{
    qDebug("Scan Stuff");
}

void MainWindow::on_importButton_clicked()
{
    qDebug("Import Stuff");
}

void MainWindow::on_actionQuit_triggered()
{
    qDebug("Closing");
    this->close();
}
