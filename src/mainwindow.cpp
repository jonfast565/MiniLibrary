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
