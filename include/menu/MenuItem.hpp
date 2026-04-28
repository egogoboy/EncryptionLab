#pragma once

#include <QPushButton>
#include <QWidget>

#include "DropdownElement.hpp"

class MenuItem : public QPushButton {
    Q_OBJECT
   public:
    explicit MenuItem(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent) {
        setFlat(true);

        setStyleSheet(R"(
            QPushButton {
                padding: 6px 12px;
                border: none;
                background: transparent;
            }

            QPushButton:hover {
                background: #e6e6e6;
            }
        )");
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

        _dropdown = new DropdownElement(parent->window());
    }

    QPushButton* addItem(const QString& text) {
        return _dropdown->addItem(text);
    }

    DropdownElement* dropdown() const { return _dropdown; }

   private:
    DropdownElement* _dropdown;
};
