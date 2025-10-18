#include "main_window.h"

#include "settings_dialog.h"
#include "ui_main_window.h"
#include "src/cvm/ws/ws_manager.h"

main_window::main_window(QWidget *parent)
: QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);
    
    connect(ui->tabs, &QTabWidget::tabCloseRequested, ui->tabs, &QTabWidget::removeTab);
    connect(ui->action_debug_button, &QAction::triggered, ui->tabs, [this]() { delete ui->tabs; });
    connect(ui->action_open_settings, &QAction::triggered, this, &main_window::on_action_open_settings_triggered);

    // Create websocket client and connect to hardcoded servers for now
    //TODO: Connect to a predefined list of servers that can be configured from within the user interface.
    cvm::ws::client_manager* manager = new cvm::ws::client_manager(ui->vm_list_view);

    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm1"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm2"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm3"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm4"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm5"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm6"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm7"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm8"));
    manager->add_client(QUrl("wss://computernewb.com/collab-vm/vm9"));

    cvm::models::vm_list* vm_list = new cvm::models::vm_list(ui->vm_list_view);

    connect(manager, &cvm::ws::client_manager::signal_list_received, vm_list, &cvm::models::vm_list::append);

    ui->vm_list_view->setModel(vm_list);
}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
}

void main_window::on_action_open_settings_triggered()
{
    settings_dialog* settings = new settings_dialog(this);

    settings->exec();
}
