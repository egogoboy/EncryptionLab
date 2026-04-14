#include "Encoding.hpp"

#include <QString>
#include <set>
#include <vector>

namespace algo {

bool validate_key(const QString& key) {
    for (int i = 0; i < key.size(); i++) {
        if (!key[i].isLetter()) {
            return false;
        }
    }

    std::set<QChar> letters;
    for (int i = 0; i < key.size(); i++) {
        auto it = letters.find(key[i]);
        if (it != letters.end()) {
            return false;
        }
        letters.insert(key[i]);
    }
    return true;
}

QString encode(const QString& text, const QString& key) {
    int cols = key.size();
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
            idx++;
        }
    }

    std::vector<std::pair<int, QChar>> indexedChars;
    for (int i = 0; i < key.length(); ++i) {
        indexedChars.push_back({i, key[i]});
    }

    std::sort(indexedChars.begin(), indexedChars.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    QString res = "";

    for (const auto& pair : indexedChars) {
        for (int i = 0; i < rows; i++) {
            res += table[i][pair.first];
        }
    }

    return res;
}

QString decode(const QString& text, const QString& key) {
    int cols = key.size();
    int rows = text.size() / cols;
    std::vector<std::vector<QChar>> table(rows, std::vector<QChar>(cols));

    std::vector<std::pair<int, QChar>> indexedChars;
    for (int i = 0; i < key.length(); ++i) {
        indexedChars.push_back({i, key[i]});
    }

    std::sort(indexedChars.begin(), indexedChars.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    int idx = 0;
    for (const auto& pair : indexedChars) {
        for (int i = 0; i < rows; i++) {
            table[i][pair.first] = text[idx];
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
