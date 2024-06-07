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

// Function to validate ISBN-10
bool isValidISBN10(const QString& isbn) {
    if (isbn.length() != 10) {
        return false;
    }

    int sum = 0;
    for (int i = 0; i < 10; ++i) {
        if (isbn[i] == 'X' && i == 9) {
            sum += 10 * (i + 1);
        } else if (isbn[i].isDigit()) {
            sum += (isbn[i].digitValue()) * (i + 1);
        } else {
            return false;
        }
    }
    return sum % 11 == 0;
}

// Function to validate ISBN-13
bool isValidISBN13(const QString& isbn) {
    if (isbn.length() != 13) {
        return false;
    }

    int sum = 0;
    for (int i = 0; i < 13; ++i) {
        if (!isbn[i].isDigit()) {
            return false;
        }
        int digit = isbn[i].digitValue();
        sum += (i % 2 == 0) ? digit : 3 * digit;
    }
    return sum % 10 == 0;
}

// Function to check if the provided ISBN is valid
bool isValidISBN(const QString& isbn) {
    return isValidISBN10(isbn) || isValidISBN13(isbn);
}
