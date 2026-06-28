#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("AIKong Modern Remote Admin - Qt + Rust Core");
    window.resize(1280, 800);
    window.show();

    return app.exec();
}