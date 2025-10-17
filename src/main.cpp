#include <QApplication>
#include <QListView>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;
    main_window.show();

    return QApplication::exec();
}
