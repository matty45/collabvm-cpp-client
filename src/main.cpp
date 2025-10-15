#include <QApplication>

#include "mainwindow.h"
#include "cvm/models/vm_list_model.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;
    main_window.show();

    //my_model.insert

    return app.exec();
}
