#include <QString>

namespace algo {

bool validate_key(const QString& key);
QString encode(const QString& input_text, const QString& key);
QString decode(const QString& input_text, const QString& key);

};  // namespace algo
