#include "Encoding.hpp"

#include <QString>

namespace algo {

bool validate_key(const QString& key) {
    if (key.length() != 5) {
        return false;
    }

    bool seen[5] = {false};
    for (int i = 0; i < 5; ++i) {
        if (!key[i].isDigit()) {
            return false;
        }
        int digit = key[i].digitValue();
        if (digit < 1 || digit > 5) {
            return false;
        }
        if (seen[digit]) {
            return false;
        }
        seen[digit] = true;
    }

    return true;
}

QString encode(const QString& text, const QString& key) {
    int d = 5;
    QString appended_text = text;
    while (appended_text.length() % d) {
        appended_text += "_";
    }

    QString result;

    for (size_t i = 0; i < appended_text.length(); i += d) {
        QString block_text;
        for (int j = 0; j < d; ++j) {
            block_text += appended_text.mid(i, 5)[key[j].digitValue() - 1];
        }
        result += block_text;
    }

    return result;
}

QString decode(const QString& text, const QString& key) {
    int d = 5;

    QString result;

    for (size_t i = 0; i < text.length(); i += d) {
        QString block_text = "00000";
        for (int j = 0; j < d; ++j) {
            block_text[key[j].digitValue() - 1] = text.mid(i, 5)[j];
        }
        result += block_text;
    }

    return result;
}

}  // namespace algo
