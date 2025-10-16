#include <QApplication>
#include <QListView>

#include "mainwindow.h"
#include "cvm/models/vm_list_model.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;

    //TODO: I don't know if this is best practice to access a widget from main.cpp
    QListView* vm_list_view = main_window.findChild<QListView*>("vm_list_view");
    Q_ASSERT(vm_list_view);

    cvm::vm_list_model* vm_list = new cvm::vm_list_model(vm_list_view);

    vm_list_view->setModel(vm_list);

    main_window.show();

    return app.exec();
}
