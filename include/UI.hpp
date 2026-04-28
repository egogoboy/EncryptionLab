#pragma once

#include <QFile>
#include <QMainWindow>
#include <cstdlib>
#include <stack>

#include "HelpWindow.hpp"
#include "KeyDialog.hpp"
#include "WorkspaceTextEdit.hpp"
#include "menu/MenuButton.hpp"
#include "menu/MenuItem.hpp"
#include "menu/TopBarMenu.hpp"

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
    TopBarElement* init_menu_bar();
    WorkspaceTextEdit* init_workspace();
    void init_ui();

    void reset_workspace();

    QFile _file;

    MenuItem* _file_menu = nullptr;
    MenuItem* _encrypt_menu = nullptr;
    MenuItem* _decrypt_menu = nullptr;
    MenuItem* _info_menu = nullptr;

    QPushButton* _save_action = nullptr;
    QPushButton* _show_help_action = nullptr;
    MenuButton* _exit_action = nullptr;

    QWidget* _window = nullptr;
    QVBoxLayout* _layout = nullptr;

    std::unique_ptr<HelpWindow> _help_window = nullptr;

    WorkspaceTextEdit* _workspace = nullptr;

    KeyDialog* _key_dialog = nullptr;

    QString (*do_action)(const QString&, const QString&);

    std::stack<QString> _text_lines;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами перестановки

Шифрование <place_your_method> методом

<place_your_full_name>, ИПБ-23)";
};
