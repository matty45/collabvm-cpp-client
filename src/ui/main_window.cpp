#include "main_window.h"

#include "ui_main_window.h"
#include "settings/settings_dialog.h"
#include "src/cvm/models/delegates/vm_delegate.h"
#include "src/cvm/ws/ws_manager.h"
#include "src/settings/settings_manager.h"
#include "vms/vm_window.h"

main_window::main_window(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::main_window)
{

	ui->setupUi(this);

	// Create settings manager
	settings_manager* s_manager = new settings_manager(this);

	// Create websocket client manager
	cvm::ws::client_manager* c_manager = new cvm::ws::client_manager(this);

	//Load persistence mode setting
	c_manager->m_persistence_mode = s_manager->get_persistence_mode();

	// Load list of servers from settings
	c_manager->m_servers = s_manager->get_servers();

	// Setup VM list.
	cvm::models::vm_list* vm_list = new cvm::models::vm_list(this);

	cvm::delegates::vm_delegate* delegate = new cvm::delegates::vm_delegate(ui->vm_list_view);
	ui->vm_list_view->setItemDelegate(delegate);

	ui->vm_list_view->setModel(vm_list);

	connect(c_manager, &cvm::ws::client_manager::signal_list_received, vm_list, &cvm::models::vm_list::append);

	// Open settings logic
	connect(ui->action_open_settings, &QAction::triggered, this, [this] {
		settings_dialog* settings = new settings_dialog(this);
		settings->setAttribute(Qt::WA_DeleteOnClose);
		settings->exec();
		});

	connect(ui->vm_list_view, &QAbstractItemView::activated, this, &main_window::on_vm_activated);

	//Refresh button logic
	connect(c_manager, &cvm::ws::client_manager::all_clients_cleared, c_manager, &cvm::ws::client_manager::connect_to_servers);
	connect(ui->action_refresh_all_servers, &QAction::triggered, this, [this, vm_list, c_manager] {

		vm_list->clear();

		if (c_manager->m_persistence_mode)
			c_manager->broadcast("4.list;");
		else
			c_manager->clear_all_clients();
		
		});

	c_manager->connect_to_servers();

}

void main_window::on_vm_activated(const QModelIndex& index) {
    cvm::models::vm_list* model = qobject_cast<cvm::models::vm_list*>(ui->vm_list_view->model());
    if (!model) return;

    cvm::vm vm_data = model->vm(index);

    // Check if window already exists  
    if (m_open_vm_windows.contains(vm_data.m_id)) {
	    if (QWidget* existing_window = m_open_vm_windows[vm_data.m_id]) {
            existing_window->raise();
            existing_window->activateWindow();
            return;
        }
    }

    // Create new window  
    vm_window* vm_w = new vm_window(vm_data);
    m_open_vm_windows[vm_data.m_id] = vm_w;

    // Clean up when window closes  
    connect(vm_w, &QWidget::destroyed, this, [this, vm_id = vm_data.m_id]() {
        m_open_vm_windows.remove(vm_id);
        });

    vm_w->show();
	vm_w->setAttribute(Qt::WA_DeleteOnClose);
	vm_w->setWindowTitle(vm_data.m_display_name);
}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
}
