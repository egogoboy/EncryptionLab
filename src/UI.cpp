#include "UI.hpp"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>

#include "Encoding.hpp"
#include "KeyDialog.hpp"
#include "menu/DropdownElement.hpp"

UI::UI(size_t height, size_t width) : QMainWindow(nullptr) {
    resize(height, width);
    setWindowTitle("Шифрование методами перестановки");
    _window = new QWidget(this);
    setCentralWidget(_window);

    init_ui();
}

void UI::show_not_implemented_warning() {
    QMessageBox* warn = new QMessageBox(_window);
    warn->setIcon(QMessageBox::Warning);
    warn->setStandardButtons(QMessageBox::NoButton);
    warn->setWindowTitle("");
    warn->setInformativeText("Указанный метод не реализован");

    QPushButton* cancel_button =
        warn->addButton(QMessageBox::StandardButton::Cancel);
    cancel_button->hide();
    warn->setEscapeButton(cancel_button);

    warn->show();

    connect(warn, &QMessageBox::finished, this, [=]() {
        _encrypt_menu->dropdown()->closeSelected();
        _decrypt_menu->dropdown()->closeSelected();
    });
}

void UI::show_about() {
    QMessageBox::information(this, "О программе", ABOUT_MESSAGE);
}

void UI::show_help() {
    _help_window->move(x() + 20, y() + 20);
    _help_window->show();
    _help_window->activateWindow();
}

void UI::encode() {
    _encrypt_menu->dropdown()->setLocked(true);
    do_action = algo::encode;
    _key_dialog->show();
    _key_dialog->raise();
    _key_dialog->activateWindow();
}

void UI::decode() {
    _decrypt_menu->dropdown()->setLocked(true);
    do_action = algo::decode;
    _key_dialog->show();
    _key_dialog->raise();
    _key_dialog->activateWindow();
}

void UI::on_workspace_changed() {
    if (_workspace->toPlainText().isEmpty()) {
        _show_help_action->setEnabled(false);
        _encrypt_menu->setEnabled(false);
        _decrypt_menu->setEnabled(false);
    } else {
        _show_help_action->setEnabled(true);
        _encrypt_menu->setEnabled(true);
        _decrypt_menu->setEnabled(true);
    }
}

void UI::on_key_received(const QString& key) {
    if (!algo::validate_key(key)) {
        show_key_error();
        return;
    }

    QString input;
    if (_text_lines.empty()) {
        input = _workspace->toPlainText();
    } else {
        input = _text_lines.top();
    }

    QString result;
    result = do_action(input, key);

    _text_lines.push(result);
    _workspace->append(result);
    _workspace->setReadOnly(true);

    _key_dialog->reset_input();
    _key_dialog->close();

    _encrypt_menu->dropdown()->setLocked(false);
    _decrypt_menu->dropdown()->setLocked(false);

    _encrypt_menu->dropdown()->closePanel();
    _decrypt_menu->dropdown()->closePanel();
}

void UI::show_key_error() {
    QMessageBox* err = new QMessageBox(this);
    err->setWindowFlag(Qt::WindowCloseButtonHint);
    err->setIcon(QMessageBox::Critical);
    err->setWindowTitle("");
    err->setText("Значение ключа неправильное");
    err->move(_key_dialog->x() + 20, _key_dialog->y() + 20);

    err->setWindowModality(Qt::ApplicationModal);

    connect(err, &QMessageBox::finished, this,
            [=]() { _key_dialog->reset_input(); });
    err->show();
}

void UI::open_file() {
    if (_file.isOpen()) {
        _file.close();
    }

    _file.setFileName(QFileDialog::getOpenFileName(
        this, tr("Открыть файл"), ".", tr("Text Files (*.txt)")));

    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("Не удалось открыть файл"),
                                 _file.errorString());
        return;
    }

    QString input_text;
    QTextStream in(&_file);
    in >> input_text;

    reset_workspace();
    _workspace->setText(input_text);

    QTextCursor cursor = _workspace->textCursor();
    cursor.movePosition(QTextCursor::EndOfLine);
    _workspace->setTextCursor(cursor);

    _save_action->setEnabled(true);
    _file.close();

    activateWindow();
}

void UI::create_file() {
    if (_file.isOpen()) {
        _file.close();
    }

    _file.setFileName("Новый файл.txt");

    _save_action->setEnabled(false);
    reset_workspace();
    activateWindow();
}

void UI::save_file() {
    if (_file.isOpen()) {
        _file.close();
    }

    if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("Не удалось открыть файл"),
                                 _file.errorString());
        return;
    }

    QTextStream out(&_file);
    if (_text_lines.empty()) {
        out << _workspace->toPlainText();
    } else {
        out << _text_lines.top();
    }
}

void UI::save_file_as() {
    if (_file.isOpen()) {
        _file.close();
    }
    _file.setFileName(QFileDialog::getSaveFileName(_window, tr("Сохранить как"),
                                                   "Новый файл.txt",
                                                   tr("Text Files (*.txt)")));
    save_file();
}

void UI::reset_workspace() {
    while (!_text_lines.empty()) {
        _text_lines.pop();
    }

    _workspace->clear();
    _workspace->setText("");
    _workspace->setVisible(true);
    _workspace->setReadOnly(false);
    _workspace->setFocus();
}

void UI::init_ui() {
    _layout = new QVBoxLayout(_window);
    _layout->setAlignment(Qt::AlignTop);
    _window->setLayout(_layout);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);

    _layout->addWidget(init_menu_bar());
    _workspace = init_workspace();
    _layout->addWidget(_workspace);

    _key_dialog = new KeyDialog(_window);
    _key_dialog->setWindowFlag(Qt::WindowStaysOnTopHint);
    connect(_key_dialog, &KeyDialog::key_submitted, this, &UI::on_key_received);

    connect(_key_dialog, &KeyDialog::finished, this, [=]() {
        _encrypt_menu->dropdown()->setLocked(false);
        _decrypt_menu->dropdown()->setLocked(false);
    });

    _help_window = std::make_unique<HelpWindow>();
}

TopBarElement* UI::init_menu_bar() {
    TopBarElement* top_bar = new TopBarElement(this);

    _file_menu = top_bar->addItem("Файл");

    connect(_file_menu->addItem("Создать"), &QPushButton::clicked, this,
            &UI::create_file);
    connect(_file_menu->addItem("Открыть"), &QPushButton::clicked, this,
            &UI::open_file);

    _save_action = _file_menu->addItem("Сохранить");
    _save_action->setEnabled(false);

    connect(_save_action, &QPushButton::clicked, this, &UI::save_file);
    connect(_file_menu->addItem("Сохранить как"), &QPushButton::clicked, this,
            &UI::save_file_as);

    _file_menu->dropdown()->addSeparator();
    connect(_file_menu->addItem("Выход"), &QPushButton::clicked, this,
            &QApplication::quit);

    _encrypt_menu = top_bar->addItem("Зашифровать");
    _encrypt_menu->setEnabled(false);
    _encrypt_menu->dropdown()->setCloseOnChoice(false);
    // _encrypt_menu->dropdown()->setSelectable(true);

    connect(_encrypt_menu->addItem("Маршрутная"), &QPushButton::clicked, this,
            &UI::show_not_implemented_warning);
    connect(_encrypt_menu->addItem("Вертикальная"), &QPushButton::clicked, this,
            &UI::show_not_implemented_warning);
    connect(_encrypt_menu->addItem("Посимвольная"), &QPushButton::clicked, this,
            &UI::show_not_implemented_warning);
    connect(_encrypt_menu->addItem("Побитовая"), &QPushButton::clicked, this,
            &UI::encode);

    _decrypt_menu = top_bar->addItem("Расшифровать");
    _decrypt_menu->setEnabled(false);
    _decrypt_menu->dropdown()->setCloseOnChoice(false);
    // _decrypt_menu->dropdown()->setSelectable(true);

    connect(_decrypt_menu->addItem("Маршрутная"), &QPushButton::clicked, this,
            &UI::show_not_implemented_warning);
    connect(_decrypt_menu->addItem("Вертикальная"), &QPushButton::clicked, this,
            &UI::show_not_implemented_warning);
    connect(_decrypt_menu->addItem("Посимвольная"), &QPushButton::clicked, this,
            &UI::show_not_implemented_warning);
    connect(_decrypt_menu->addItem("Побитовая"), &QPushButton::clicked, this,
            &UI::decode);

    _info_menu = top_bar->addItem("Справка");

    connect(_info_menu->addItem("О программе"), &QPushButton::clicked, this,
            &UI::show_about);

    _show_help_action = _info_menu->addItem("Помощь");
    _show_help_action->setEnabled(false);
    connect(_show_help_action, &QPushButton::clicked, this, &UI::show_help);

    MenuButton* exit_btn = top_bar->addButton("Выход");
    connect(exit_btn, &QPushButton::clicked, this, &QApplication::quit);

    return top_bar;
}

WorkspaceTextEdit* UI::init_workspace() {
    WorkspaceTextEdit* workspace = new WorkspaceTextEdit(_window);
    workspace->setReadOnly(true);
    workspace->setVisible(false);
    connect(workspace, &QTextEdit::textChanged, this,
            &UI::on_workspace_changed);

    return workspace;
}
