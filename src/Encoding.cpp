#include "Encoding.hpp"

#include <QString>
#include <vector>

namespace algo {
const QString ABC_ = QString::fromStdString("_АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");

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
        if (digit < 0 || digit > 4) {
            return false;
        }
        if (seen[digit]) {
            return false;
        }
        seen[digit] = true;
    }

    return true;
}

QString letterToBin(QChar letter) {
    return QString::number(ABC_.indexOf(letter), 2).rightJustified(5, '0');
}

QChar binToLetter(const QString& bin) {
    bool ok;
    int result = bin.toInt(&ok, 2);

    return ABC_[result];
}

QString encode(const QString& text, const QString& key) {
    std::vector<QString> bin_letters;

    for (int i = 0; i < text.length(); ++i) {
        QString bin_letter = letterToBin(text[i]);
        bin_letters.emplace_back(bin_letter);
    }

    QString result;
    for (const auto& letter : bin_letters) {
        QString crypted_letter;
        for (int i = 0; i < 5; ++i) {
            crypted_letter += letter[4 - key[i].digitValue()];
        }

        result += binToLetter(crypted_letter);
    }

    return result;
}

QString decode(const QString& text, const QString& key) {
    int inverse_order[5];
    QString inv_key = "00000";
    for (int i = 0; i < 5; ++i) {
        int from = 4 - key[i].digitValue();
        inverse_order[from] = i;
        inv_key[from] = i + '0';
    }

    QString result;

    for (int i = 0; i < text.length(); ++i) {
        QString encrypted_bin = letterToBin(text[i]);

        QString decrypted_bin;
        for (int j = 0; j < 5; ++j) {
            decrypted_bin += encrypted_bin[inverse_order[j]];
        }

        result += binToLetter(decrypted_bin);
    }

    return result;
}

}  // namespace algo
