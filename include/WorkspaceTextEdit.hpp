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

        if (event->text().contains(QRegExp("[^А-ИК-Яа-ик-я_ \b]")) ||
            toPlainText().size() >= 30) {
            return;
        }

        if (event->key() == Qt::Key_Space) {
            QKeyEvent* new_event = new QKeyEvent(
                QEvent::KeyPress, Qt::Key_Underscore, event->modifiers(), "_");

            QTextEdit::keyPressEvent(new_event);
            return;
        }

        if (event->text().contains(QRegExp("[а-ик-я]"))) {
            QKeyEvent* new_event =
                new QKeyEvent(QEvent::KeyPress, event->key(),
                              event->modifiers(), event->text().toUpper());

            QTextEdit::keyPressEvent(new_event);
            return;
        }

        QTextEdit::keyPressEvent(event);
    }
};
