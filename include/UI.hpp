#pragma once

#include <QFile>
#include <QMainWindow>
#include <cstdlib>
#include <stack>

#include "HelpWindow.hpp"
#include "KeyDialog.hpp"
#include "WorkspaceTextEdit.hpp"

class UI : public QMainWindow {
   public:
    explicit UI(size_t height, size_t width);

   private slots:
    void show_not_implemented_warning();
    void show_about();
    void show_help();
    void show_key_error();

    void encode();
    void decode();

    void open_file();
    void create_file();
    void save_file();
    void save_file_as();

    void on_workspace_changed();
    void on_key_received(const QString& key);

   private:
    void init_menu_bar();
    void init_workspace();
    void init_ui();

    void reset_workspace();

    QFile _file;

    QMenu* _file_menu = nullptr;
    QMenu* _encrypt_menu = nullptr;
    QMenu* _decrypt_menu = nullptr;
    QMenu* _info_menu = nullptr;

    QAction* _save_action = nullptr;
    QAction* _show_help_action = nullptr;
    QAction* _exit_action = nullptr;

    QWidget* _window = nullptr;
    QLayout* _layout = nullptr;

    std::unique_ptr<HelpWindow> _help_window = nullptr;

    WorkspaceTextEdit* _workspace = nullptr;

    KeyDialog* _key_dialog = nullptr;

    QString (*do_action)(const QString&, const QString&);

    std::stack<QString> _text_lines;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами перестановки

Шифрование методом вертикальной перестановки с ключом

<place_your_full_name>, ИПБ-23)";
};
