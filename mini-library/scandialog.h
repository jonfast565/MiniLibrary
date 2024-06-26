#ifndef SCANDIALOG_H
#define SCANDIALOG_H

#include <QDialog>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

#include <QErrorMessage>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QStandardItemModel>

#include "models.h"
#include "databasehandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ScanDialog;
}
QT_END_NAMESPACE

class ScanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScanDialog(QWidget *parent = nullptr);
    ~ScanDialog();

    Models::Book getCurrentBook();
    void resetDialog();

private:
    Ui::ScanDialog *ui;
    QNetworkAccessManager manager;
    QStandardItemModel* searchTableModel;
    DatabaseHandler* databaseHandler;
    Models::Book currentBook;

    void initDialog();
    void resetProgressAndHide();
    void clearResultsSection();
    void setProgressIndeterminate();
    void disableSearchControls();
    void enableSearchControls();
    void selectFirstRow();

private slots:
    void requestBook();
    void updateResultsSection(QList<Models::Book>& books);
    void setResults();
    void setScanValues(Models::Book& book);
    void setCurrentBook(Models::Book& book);
    void clearSearchField();
    void clearSearchResults();
    void searchRequestAuto(const QString& searchTerm);
};

#endif // SCANDIALOG_H
