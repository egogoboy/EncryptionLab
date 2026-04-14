#include "Encoding.hpp"

#include <QString>

namespace algo {

const QString alphabet = "_АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

size_t get_char_idx(QChar c) { return alphabet.indexOf(c); }

bool validate_key(const QString& key) {
    bool ok = false;
    key.toInt(&ok);
    return ok;
}

QString encode(const QString& input_text, const QString& key) {
    QString result;
    int key_value = key.toInt();
    result.reserve(input_text.size());

    for (QChar c : input_text) {
        int idx =
            (static_cast<int>(get_char_idx(c)) + key_value) % alphabet.size();

        if (idx < 0) {
            idx = static_cast<int>(alphabet.size()) + idx;
        }
        result.push_back(alphabet.at(idx));
    }

    return result;
}

QString decode(const QString& input_text, const QString& key) {
    QString result;
    int key_value = key.toInt();
    result.reserve(input_text.size());

    for (QChar c : input_text) {
        int idx =
            (static_cast<int>(get_char_idx(c)) - key_value) % alphabet.size();

        if (idx < 0) {
            idx = static_cast<int>(alphabet.size()) + idx;
        }
        result.push_back(alphabet.at(idx));
    }

    return result;
}

};  // namespace algo
