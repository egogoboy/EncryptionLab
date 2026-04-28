#pragma once

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWidget>
#include <vector>

#include "MenuButton.hpp"
#include "MenuItem.hpp"

class TopBarElement : public QWidget {
    Q_OBJECT
   public:
    explicit TopBarElement(QWidget* parent = nullptr) : QWidget(parent) {
        _layout = new QHBoxLayout(this);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setAlignment(Qt::AlignLeft);
        qApp->installEventFilter(this);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

        setMouseTracking(true);
    }

    MenuItem* addItem(const QString& text) {
        MenuItem* item = new MenuItem(text, this);
        item->installEventFilter(this);
        _layout->addWidget(item);

        _items.push_back(item);

        connect(item, &QPushButton::clicked, [=]() {
            if (_current == item && _menu_active) {
                closeAll();
            } else {
                openMenu(item);
            }
        });

        connect(item->dropdown(), &DropdownElement::closed, this, [=]() {
            if (_current == item) {
                _current = nullptr;
                _menu_active = false;
            }
        });

        return item;
    }

    MenuButton* addButton(const QString& text) {
        MenuButton* item = new MenuButton(text, this);
        item->installEventFilter(this);
        _layout->addWidget(item);

        _buttons.push_back(item);

        return item;
    }

    void openMenu(MenuItem* item) {
        for (auto* i : _items) {
            if (i != item) i->dropdown()->closePanel();
        }

        _current = item;
        _menu_active = true;

        item->dropdown()->openBelow(item);
    }

    void closeAll() {
        for (auto* i : _items) {
            i->dropdown()->closePanel();
        }
        _current = nullptr;
        _menu_active = false;
    }

    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::Enter) {
            auto* btn = qobject_cast<MenuItem*>(obj);
            if (btn && btn->isEnabled() && _menu_active) {
                openMenu(btn);
            }

            if (auto* btn = qobject_cast<MenuButton*>(obj)) {
                if (_menu_active) {
                    closeAll();
                    _menu_active = true;
                }
            }
        }

        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouse = static_cast<QMouseEvent*>(event);
            QWidget* clicked = QApplication::widgetAt(mouse->globalPos());

            bool inside_menu = false;

            for (auto* item : _items) {
                if (item == clicked || item->isAncestorOf(clicked) ||
                    item->dropdown()->isAncestorOf(clicked)) {
                    inside_menu = true;
                    break;
                }
            }

            if (!inside_menu && _menu_active) {
                if (_current) {
                    if (auto* dd = dynamic_cast<MenuItem*>(_current)) {
                        dd->dropdown()->closePanel();
                    }
                } else {
                    closeAll();
                }
            }
        }

        return QWidget::eventFilter(obj, event);
    }

   private:
    QHBoxLayout* _layout = nullptr;
    QObject* _current = nullptr;
    std::vector<MenuItem*> _items;
    std::vector<MenuButton*> _buttons;
    bool _menu_active = false;
};
