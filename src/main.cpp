#include <QApplication>

#include "ui/main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;
    main_window.show();

    return QApplication::exec();
}
