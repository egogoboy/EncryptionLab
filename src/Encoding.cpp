#include "Encoding.hpp"

#include <QString>
#include <iostream>
#include <vector>

namespace algo {

bool validate_key(const QString& key) {
    for (int i = 0; i < key.size(); i++) {
        if (!key[i].isDigit()) {
            return false;
        }
    }

    bool ok;
    int digit_key = key.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (digit_key < 1) {
        return false;
    }

    return true;
}

QString encode(const QString& text, const QString& key) {
    int cols = key.toInt();
    int rows = std::ceil((double)text.size() / (double)cols);
    std::vector<std::vector<QChar>> table(rows, std::vector<QChar>(cols));

    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (idx >= text.size()) {
                table[i][j] = '_';
            } else {
                table[i][j] = text[idx];
            }
            std::cout << QString(table[i][j]).toStdString() << " ";
            idx++;
        }
        std::cout << std::endl;
    }

    QString res = "";

    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            res += table[i][j];
        }
    }

    return res;
}

QString decode(const QString& text, const QString& key) {
    int cols = key.toInt();
    int rows = text.size() / cols;
    std::vector<std::vector<QChar>> table(rows, std::vector<QChar>(cols));

    int idx = 0;
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            table[i][j] = text[idx];
            idx++;
        }
    }

    QString res = "";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res += table[i][j];
        }
    }

    // Если нужно будет убрать пробелы в конце строки
    /*for (int i = res.size() - 1; i >= 0; i--) {
        if (res[i] == '_') {
            res.resize(res.size() - 1);
        }
        else {
            break;
        }
    }*/

    return res;
}

}  // namespace algo
