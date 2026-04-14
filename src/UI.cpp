#include "UI.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>

#include "Encoding.hpp"
#include "KeyDialog.hpp"

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
}

void UI::show_about() {
    QMessageBox::information(this, "О программе", ABOUT_MESSAGE);
}

void UI::show_help() {
    _help_window->show();
    _help_window->activateWindow();
}

void UI::encode() {
    do_action = algo::encode;
    _key_dialog->show();
    _key_dialog->raise();
    _key_dialog->activateWindow();
}

void UI::decode() {
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
        _key_dialog->reset_input();
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
}

void UI::show_key_error() {
    QMessageBox* err = new QMessageBox(this);
    err->setWindowFlag(Qt::WindowCloseButtonHint);
    err->setIcon(QMessageBox::Critical);
    err->setWindowTitle("");
    err->setText("Значение ключа неправильное");

    err->setWindowModality(Qt::ApplicationModal);
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
    init_menu_bar();
    init_workspace();

    _key_dialog = new KeyDialog(_window);
    _key_dialog->setWindowFlag(Qt::WindowStaysOnTopHint);
    connect(_key_dialog, &KeyDialog::key_submitted, this, &UI::on_key_received);

    _help_window = std::make_unique<HelpWindow>();
}

void UI::init_menu_bar() {
    menuBar()->setNativeMenuBar(false);

    _file_menu = menuBar()->addMenu(QObject::tr("Файл"));
    connect(_file_menu->addAction("Создать"), &QAction::triggered, this,
            &UI::create_file);
    connect(_file_menu->addAction("Открыть"), &QAction::triggered, this,
            &UI::open_file);

    _save_action = _file_menu->addAction("Сохранить");
    _save_action->setEnabled(false);

    connect(_save_action, &QAction::triggered, this, &UI::save_file);
    connect(_file_menu->addAction("Сохранить как"), &QAction::triggered, this,
            &UI::save_file_as);

    _file_menu->addSeparator();
    connect(_file_menu->addAction("Выход"), &QAction::triggered, this,
            &QApplication::quit);

    _encrypt_menu = menuBar()->addMenu(QObject::tr("Зашифровать"));
    _encrypt_menu->setEnabled(false);

    // <choose_active_method>

    connect(_encrypt_menu->addAction("Маршрутная"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);
    connect(_encrypt_menu->addAction("Вертикальная"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);
    connect(_encrypt_menu->addAction("Посимвольная"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);
    connect(_encrypt_menu->addAction("Побитовая"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);

    _decrypt_menu = menuBar()->addMenu(QObject::tr("Расшифровать"));
    _decrypt_menu->setEnabled(false);

    // <choose_active_method>

    connect(_decrypt_menu->addAction("Маршрутная"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);
    connect(_decrypt_menu->addAction("Вертикальная"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);
    connect(_decrypt_menu->addAction("Посимвольная"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);
    connect(_decrypt_menu->addAction("Побитовая"), &QAction::triggered, this,
            &UI::show_not_implemented_warning);

    _info_menu = menuBar()->addMenu(QObject::tr("Справка"));
    connect(_info_menu->addAction("О программе"), &QAction::triggered, this,
            &UI::show_about);

    _show_help_action = _info_menu->addAction("Помощь");
    _show_help_action->setEnabled(false);
    connect(_show_help_action, &QAction::triggered, this, &UI::show_help);

    connect(menuBar()->addAction(QObject::tr("Выход")), &QAction::triggered,
            this, &QApplication::quit);
}

void UI::init_workspace() {
    _layout = new QHBoxLayout(_window);
    _window->setLayout(_layout);

    _workspace = new WorkspaceTextEdit(_window);
    _workspace->setReadOnly(true);
    _workspace->setVisible(false);
    connect(_workspace, &QTextEdit::textChanged, this,
            &UI::on_workspace_changed);

    _layout->addWidget(_workspace);
}
