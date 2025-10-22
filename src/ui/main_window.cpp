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
	//m_vm_list = new cvm::models::vm_list(this);

	for (QUrl url : m_settings_manager->get_servers())
	{
		m_server_manager->add_server(url);
	}


	// Connect each server's vm_added signal to the model  
	for (cvm::server* server : m_server_manager->servers()) {
		//connect(server, &cvm::server::vm_added, this, [cvm::models::vm_list, server](cvm::vm* vm) {
		//	m_vm_list->append(vm->m_id, vm->m_display_name,
		//		QString::fromUtf8(vm->m_thumbnail.toImage().constBits()));
		//	});

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
