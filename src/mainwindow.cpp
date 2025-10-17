#include "mainwindow.h"
#include "ui_mainwindow.h"

main_window::main_window(QWidget *parent)
: QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);
    
    connect(ui->tabs, &QTabWidget::tabCloseRequested, ui->tabs, &QTabWidget::removeTab);

    // UGLY C
    // Create websocket client and connect to hardcoded servers for now
    //TODO: Connect to a predefined list of servers that can be configured from within the user interface.
    cvm::ws::client* vm1 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm1"), this);
    cvm::ws::client* vm2 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm2"), this);
    cvm::ws::client* vm3 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm3"), this);
    cvm::ws::client* vm4 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm4"), this);
    cvm::ws::client* vm5 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm5"), this);
    cvm::ws::client* vm6 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm6"), this);
    cvm::ws::client* vm7 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm7"), this);
    cvm::ws::client* vm8 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm8"), this);
    cvm::ws::client* vm9 = new cvm::ws::client(QUrl("wss://computernewb.com/collab-vm/vm9"), this);

    cvm::models::vm_list* vm_list = new cvm::models::vm_list(ui->vm_list_view);

    connect(vm1, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm2, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm3, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm4, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm5, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm6, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm7, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm8, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);
    connect(vm9, &cvm::ws::client::signal_list_received, vm_list, &cvm::models::vm_list::append);

    connect(ui->action_debug_button, &QAction::triggered, vm1,&cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm2, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm3, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm4, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm5, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm6, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm7, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm8, &cvm::ws::client::close);
    connect(ui->action_debug_button, &QAction::triggered, vm9, &cvm::ws::client::close);

    ui->vm_list_view->setModel(vm_list);
}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
    
}
