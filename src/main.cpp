#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;
    main_window.show();

    return app.exec();
}
