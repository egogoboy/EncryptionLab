#pragma once

#include <QKeyEvent>
#include <QTextEdit>

class WorkspaceTextEdit : public QTextEdit {
   public:
    explicit WorkspaceTextEdit(QWidget* parent = nullptr) : QTextEdit(parent) {}

   protected:
    void keyPressEvent(QKeyEvent* event) override {
        if (event->key() == Qt::Key_Backspace) {
            QTextEdit::keyPressEvent(event);
            return;
        }

        if (event->text().contains(QRegExp("[^А-ИК-Я_\b]")) ||
            toPlainText().size() >= 30) {
            return;
        }

        QTextEdit::keyPressEvent(event);
    }
};
