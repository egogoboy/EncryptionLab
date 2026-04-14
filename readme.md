# 4 лабораторная по Михайлову

Для того, чтобы поменять под себя необходимо:

1. Поменять текст "О программе" в последних строчках файла `include/UI.hpp`
2. Поменять следующие методы в файле `src/Encoding.cpp`:
    1. `bool validate_key(const QString& key)`
    2. `QString encode(const QString& text, const QString& key)`
    3. `QString decode(const QString& text, const QString& key)`
3. Поменять текст помощи в файле `include/HelpWindow.hpp`
4. Поменять активный метод в функции `init_menu_bar()` в файле `src/UI.cpp`:
    1. Найти раздел с пунктами **Зашифровать** и **Расшифровать**
    2. Поменять в активном методе функцию в `connect` на `&UI::encode` или `&UI::decode` соответственно
    3. У некативных методов установить в `connect` функцию `&UI::show_not_implemented_warning`
        Пример:
        ```Cpp
    connect(_encrypt_menu->addAction("Маршрутная"), &QAction::triggered,
            this, &UI::encode);
    connect(_encrypt_menu->addAction("Вертикальная"), &QAction::triggered,
            this, &UI::show_not_implemented_warning);
        ```

## Где всё это менять

Все места, где нужно что либо поменять, помечены тектом типа `<place_your_variant>`
