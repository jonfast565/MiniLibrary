#ifndef UTILITIES_H
#define UTILITIES_H

#include <QList>
#include <QString>
#include <QJsonArray>
#include <QJsonValue>
#include <QStandardItem>
#include <QStandardItemModel>


QList<QString> jsonArrayToQList(const QJsonArray& jsonArray);
QList<QStandardItem*> getQStandardItemsForRow(const QStandardItemModel* model, int row);

#endif // UTILITIES_H
