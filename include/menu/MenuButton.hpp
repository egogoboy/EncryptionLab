#pragma once

#include <QPushButton>

class MenuButton : public QPushButton {
    Q_OBJECT
   public:
    explicit MenuButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent) {
        setFlat(true);
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        setStyleSheet(R"(
            QPushButton {
                padding: 3px 10px;
                border: none;
                background: transparent;
            }

            QPushButton:hover {
                background: #e6e6e6;
            }
        )");
    }
};
