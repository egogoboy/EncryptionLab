#include "KeyDialog.hpp"

#include <QHBoxLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QVBoxLayout>

KeyDialog::KeyDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Ключ");
    setModal(false);

    setWindowFlag(Qt::WindowCloseButtonHint);

    _input = new QLineEdit(this);

    QRegExp rx("-?\\d+|[А-ИК-Я_]+");
    _input->setValidator(new QRegExpValidator(rx, this));

    auto* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(_input);

    auto* buttons_layout = new QHBoxLayout();

    _submit_button = new QPushButton("OK", this);
    buttons_layout->addWidget(_submit_button);
    connect(_submit_button, &QPushButton::clicked, this, &KeyDialog::on_submit);

    _reject_button = new QPushButton("ESC", this);
    buttons_layout->addWidget(_reject_button);
    connect(_reject_button, &QPushButton::clicked, this, &KeyDialog::on_reject);

    main_layout->addLayout(buttons_layout);
}

void KeyDialog::reset_input() {
    _input->clear();
    _input->setFocus();
}

void KeyDialog::on_submit() { emit key_submitted(_input->text()); }

void KeyDialog::on_reject() {
    reset_input();
    close();
}
