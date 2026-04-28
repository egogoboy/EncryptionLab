#pragma once

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class DropdownElement : public QWidget {
    Q_OBJECT
   public:
    explicit DropdownElement(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_StyledBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setAttribute(Qt::WA_TransparentForMouseEvents, false);

        _layout = new QVBoxLayout(this);
        _layout->setContentsMargins(2, 2, 2, 2);
        _layout->setSpacing(0);

        setStyleSheet(R"(
            DropdownElement {
                background: white;
                border: 1px solid #c0c0c0;
            }
        )");

        hide();
    }

    QPushButton* addItem(const QString& text) {
        auto* btn = new QPushButton(text);
        btn->setFlat(true);
        btn->setCheckable(true);

        btn->setStyleSheet(R"(
            QPushButton {
                text-align: left;
                padding: 3px 10px;
                border: none;
                background: transparent;
            }

            QPushButton:hover {
                background: #e6e6e6;
            }

            QPushButton:checked {
                background: #d0d0d0;
            }
        )");

        btn->setLayoutDirection(Qt::LeftToRight);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            if (_selectable) {
                if (_selected && _selected != btn) {
                    _selected->setChecked(false);
                }

                _selected = btn;
                btn->setChecked(true);
            } else {
                btn->setChecked(false);
            }

            if (!_locked && _close_on_choice) {
                closePanel();
            }
            emit choice();
        });

        _layout->addWidget(btn);
        return btn;
    }

    void addSeparator() {
        QWidget* sep = new QWidget(this);
        sep->setFixedHeight(1);
        sep->setStyleSheet("background: #c0c0c0;");

        _layout->addWidget(sep);
    }

    void openBelow(QWidget* anchor) {
        QPoint p = anchor->mapTo(parentWidget(), QPoint(0, anchor->height()));

        adjustSize();

        move(p);
        show();
        raise();
    }

    void closePanel() {
        if (!isVisible()) return;

        if (_locked) return;

        if (_selectable && _selected) {
            _selected->setChecked(false);
        }

        hide();
        emit closed();
    }

    void setCloseOnChoice(bool close) { _close_on_choice = close; }

    void setLocked(bool locked) { _locked = locked; }

    void setSelectable(bool selectable) { _selectable = selectable; }

    void closeSelected() {
        if (_selectable && _selected) {
            _selected->setChecked(false);
            _selected = nullptr;
        }
    }

   signals:
    void closed();
    void choice();

   private:
    QVBoxLayout* _layout = nullptr;
    QPushButton* _selected = nullptr;
    QWidget* _anchor = nullptr;
    bool _locked = false;
    bool _close_on_choice = true;
    bool _selectable = false;
};
