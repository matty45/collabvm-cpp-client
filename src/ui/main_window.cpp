#include "main_window.h"

#include <QMessageBox>

#include "ui_main_window.h"
#include "settings/settings_dialog.h"
#include "src/cvm/models/user_list.h"
#include "src/cvm/models/delegates/vm.h"
#include "src/cvm/ws/ws_manager.h"
#include "src/settings/settings_manager.h"
#include "vms/vm_window.h"

main_window::main_window(QWidget* parent)
	: QMainWindow(parent), m_ui(new Ui::main_window)
{

	m_ui->setupUi(this);

	// Create settings manager
	m_s_manager = new settings_manager(this);

	// Create websocket client manager
	m_c_manager = new cvm::ws::client_manager(this);

	//Load persistence mode setting
	m_c_manager->m_persistence_mode = m_s_manager->get_persistence_mode();

	// Load list of servers from settings
	m_c_manager->m_servers = m_s_manager->get_servers();

	// Setup VM list.
	m_vm_list = new cvm::models::vm_list(this);

	// Setup user list.
	m_user_list = new cvm::models::user_list(this);

	cvm::delegates::vm* delegate = new cvm::delegates::vm(m_ui->vm_list_view);
	m_ui->vm_list_view->setItemDelegate(delegate);

	m_ui->vm_list_view->setModel(m_vm_list);

	connect(m_c_manager, &cvm::ws::client_manager::signal_list_received, m_vm_list, &cvm::models::vm_list::append);
	connect(m_c_manager, &cvm::ws::client_manager::signal_adduser_received,m_user_list, &cvm::models::user_list::append);
	connect(m_c_manager, &cvm::ws::client_manager::signal_remuser_received, m_user_list, &cvm::models::user_list::remove);
	connect(m_c_manager, &cvm::ws::client_manager::signal_flag_received, m_user_list, &cvm::models::user_list::set_country);

	// Open settings logic
	connect(m_ui->action_open_settings, &QAction::triggered, this, [this] {
		settings_dialog* settings = new settings_dialog(this);
		settings->setAttribute(Qt::WA_DeleteOnClose);
		settings->exec();
		});

	connect(m_ui->vm_list_view, &QAbstractItemView::activated, this, &main_window::on_vm_activated);

	//Refresh button logic
	connect(m_c_manager, &cvm::ws::client_manager::all_clients_cleared, m_c_manager, &cvm::ws::client_manager::connect_to_servers);
	connect(m_ui->action_refresh_all_servers, &QAction::triggered, this, [this] {

		if (!m_open_vm_windows.empty()) // https://github.com/matty45/collabvm-qt-client/issues/15
		{
			QMessageBox::warning(
				nullptr,
				"Cannot refresh!",
				"Please close all running vm windows before refreshing the vm list.",
				QMessageBox::Ok
			);

			return;
		}

		m_vm_list->clear();

		if (m_c_manager->m_persistence_mode)
			m_c_manager->broadcast("4.list;");
		else
		{
			m_user_list->clear();
			m_c_manager->clear_all_clients();
		}
		
		});

	m_c_manager->connect_to_servers();

}

void main_window::on_vm_activated(const QModelIndex& index) {
    cvm::models::vm_list* model = qobject_cast<cvm::models::vm_list*>(m_ui->vm_list_view->model());
    if (!model) return;

    cvm::vm vm_data = model->vm_at_index(index);

    // Check if window already exists  
    if (m_open_vm_windows.contains(vm_data.m_id)) {
	    if (QWidget* existing_window = m_open_vm_windows[vm_data.m_id]) {
            existing_window->raise();
            existing_window->activateWindow();
            return;
        }
    }

	if (!m_c_manager->m_persistence_mode)
	{
		m_user_list->clear();
		m_c_manager->add_client(vm_data.m_server);
	}


    // Create new window  
    vm_window* vm_w = new vm_window(vm_data,m_user_list);
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
