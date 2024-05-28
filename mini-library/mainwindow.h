#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QSortFilterProxyModel>

#include "scandialog.h"
#include "databasehandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addBookButtonClicked();
    void refreshBooks();
    void filterTextChanged(const QString& text);
    void onSearchTableItemEdited(const QModelIndex &index);
    void clearSearch();

private:
    Ui::MainWindow *ui;
    ScanDialog *scanDialog;
    DatabaseHandler *databaseHandler;
    QStandardItemModel* searchTableModel;
    QSortFilterProxyModel* proxyModel;
};
#endif // MAINWINDOW_H
