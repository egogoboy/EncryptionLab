#include <QApplication>

#include "UI.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    UI ui(800, 600);
    ui.show();

    return app.exec();
}
