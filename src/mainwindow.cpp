#include "mainwindow.h"
#include "ui_mainwindow.h"

main_window::main_window(QWidget *parent)
: QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);
    
    connect(ui->tabs, &QTabWidget::tabCloseRequested, ui->tabs, &QTabWidget::removeTab);

    // Create websocket client and connect to hardcoded server for now
    //TODO: Connect to a predefined list of servers that can be configured from within the user interface.
    cvm::ws::client* main_client = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm9"), this);

    cvm::models::vm_list* vm_list = new cvm::models::vm_list(ui->vm_list_view);

    main_window::connect(main_client, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);

    ui->vm_list_view->setModel(vm_list);

}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
    
}
