#include <QApplication>
#include <QListView>

#include "mainwindow.h"
#include "cvm/ws.h"
#include "cvm/models/vm_list.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;
    main_window.show();

    return QApplication::exec();
}
