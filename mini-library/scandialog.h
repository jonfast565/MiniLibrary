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

private:
    Ui::ScanDialog *ui;
    QNetworkAccessManager manager;
    QStandardItemModel* searchTableModel;
    Models::Book currentBook;

    void initDialog();
    void resetProgressAndHide();
    void clearSearchField();
    void clearResultsSection();
    void setProgressIndeterminate();
    void disableSearchControls();
    void enableSearchControls();

private slots:
    void requestBook();
    void updateResultsSection(QList<Models::Book>& books);
    void setResults();
    void setScanValues(Models::Book& book);
    void setCurrentBook(Models::Book& book);
};

#endif // SCANDIALOG_H
