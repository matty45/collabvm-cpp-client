#include "main_window.h"

#include <QMessageBox>

#include "ui_main_window.h"
#include "src/cvm/models/delegates/vm.h"
#include "src/settings/settings_manager.h"

main_window::main_window(QWidget* parent)
	: QMainWindow(parent), m_ui(new Ui::main_window)
{

	m_ui->setupUi(this);

	// Create settings manager
	m_settings_manager = new settings_manager(this);

	// Create server manager
	m_server_manager = new cvm::server_manager(this);

	// Setup VM list.
	m_vm_list_model = new cvm::models::vm_list(this);
	cvm::delegates::vm* delegate = new cvm::delegates::vm(m_ui->vm_list_view);
	m_ui->vm_list_view->setItemDelegate(delegate);
	m_ui->vm_list_view->setModel(m_vm_list_model);

	for (QUrl url : m_settings_manager->get_servers())
	{
		m_server_manager->add_server(url);
	}


	// Connect each server's vm_added signal to the model  
	for (cvm::server* server : m_server_manager->servers()) {
		// Connect server signals to update the VM list  
		connect(server, &cvm::server::vm_added, m_vm_list_model, &cvm::models::vm_list::append);
		connect(server, &cvm::server::vm_removed, m_vm_list_model, &cvm::models::vm_list::remove);

		// Connect to server  
		server->connect_to_server();
	}

}

void main_window::on_vm_activated(const QModelIndex& index) {
}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
}
