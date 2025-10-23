#include "main_window.h"

#include <QMessageBox>

#include "ui_main_window.h"
#include "src/cvm/models/delegates/vm.h"
#include "src/settings/settings_manager.h"
#include "vms/vm_window.h"

main_window::main_window(QWidget* parent)
	: QMainWindow(parent), m_ui(new Ui::main_window)
{

	m_ui->setupUi(this);


	// Create settings manager
	m_settings_manager = new settings_manager(this);

	// Create server manager
	m_server_manager = new cvm::server_manager(this);

	//Load persistence mode setting
	m_server_manager->m_persistence_mode = m_settings_manager->get_persistence_mode();

	// Setup vm opening
	connect(m_ui->vm_list_view, &QAbstractItemView::activated, this, &main_window::on_vm_activated);

	//TODO: Need to disable the refresh button while servers are refreshing.
	connect(m_ui->action_refresh_all_servers, &QAction::triggered, [this]()
		{
			m_vm_list_model->clear();

			if (!m_server_manager->m_persistence_mode)
				m_server_manager->reconnect_all();
			else
				m_server_manager->broadcast_message_to_all_servers("4.list;");
		});

	// Setup VM list.
	m_vm_list_model = new cvm::models::vm_list(this);
	cvm::delegates::vm* delegate = new cvm::delegates::vm(m_ui->vm_list_view);
	m_ui->vm_list_view->setItemDelegate(delegate);
	m_ui->vm_list_view->setModel(m_vm_list_model);

	for (QUrl url : m_settings_manager->get_servers())
	{
		m_server_manager->add_server(url);
	}


	for (cvm::server* server : m_server_manager->servers()) {
		// Connect server signals to update the VM list  
		connect(server, &cvm::server::vm_added, m_vm_list_model, &cvm::models::vm_list::append);
		connect(server, &cvm::server::vm_removed, m_vm_list_model, &cvm::models::vm_list::remove);

		// Connect to server  
		server->connect_to_server();
	}
}

void main_window::on_vm_activated(const QModelIndex& index) {
	cvm::vm* vm_data = m_vm_list_model->vm_at_index(index);

	// Check if window already exists  
	if (m_open_vm_windows.contains(vm_data->m_id)) {
		if (QWidget* existing_window = m_open_vm_windows[vm_data->m_id]) {
			existing_window->raise();
			existing_window->activateWindow();
			return;
		}
	}

	if (!m_server_manager->m_persistence_mode)
	{
		vm_data->m_server->connect_to_server(true);
	}
		

	// Create new window  
	vm_window* vm_w = new vm_window(vm_data, m_server_manager->m_persistence_mode);
	m_open_vm_windows[vm_data->m_id] = vm_w;

	// Clean up when window closes  
	connect(vm_w, &QWidget::destroyed, this, [this, vm_id = vm_data->m_id]() {
		m_open_vm_windows.remove(vm_id);
		});

	vm_w->show();
	vm_w->setAttribute(Qt::WA_DeleteOnClose);
	vm_w->setWindowTitle(vm_data->m_display_name);
}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
}
