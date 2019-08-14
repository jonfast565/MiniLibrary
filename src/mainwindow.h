#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>
#include <QtSql/QSqlError>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_scanButton_clicked();
    void on_importButton_clicked();
    void on_actionQuit_triggered();

private:
    void setupInternal();
    void setupDatabaseConnection(QString server, int port, QString database, QString username, QString password);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
