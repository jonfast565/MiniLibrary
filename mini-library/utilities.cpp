#include "utilities.h"

QList<QString> jsonArrayToQList(const QJsonArray& jsonArray) {
    QList<QString> stringList;
    for (const QJsonValue& value : jsonArray) {
        if (value.isString()) {
            stringList.append(value.toString());
        }
    }
    return stringList;
}

QList<QStandardItem*> getQStandardItemsForRow(const QStandardItemModel* model, int row) {
    QList<QStandardItem*> items;
    for (auto i = 0; i < model->columnCount(); i++) {
        QStandardItem* item = model->item(row, i);
        items.push_back(item);
    }
    return QList(items);
}
