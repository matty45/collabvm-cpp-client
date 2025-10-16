#include <QApplication>
#include <QListView>

#include "mainwindow.h"
#include "cvm/ws.h"
#include "cvm/models/vm_list.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;

    // Create websocket client and connect to hardcoded server for now
	//TODO: Connect to a predefined list of servers that can be configured from within the user interface.
    cvm::ws::client* main_client = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm9"), &main_window);

    //TODO: I don't know if this is best practice to access a widget from main.cpp
    QListView* vm_list_view = main_window.findChild<QListView*>("vm_list_view");
    Q_ASSERT(vm_list_view);

    cvm::models::vm_list* vm_list = new cvm::models::vm_list(vm_list_view);

    main_window::connect(main_client, &cvm::ws::client::on_list_received, vm_list, &cvm::models::vm_list::append);

    vm_list_view->setModel(vm_list);

    main_window.show();

    return QApplication::exec();
}
