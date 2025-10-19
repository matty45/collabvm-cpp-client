#include "main_window.h"

#include "ui_main_window.h"
#include "settings/settings_dialog.h"
#include "src/cvm/ws/ws_manager.h"
#include "src/settings/settings_manager.h"

main_window::main_window(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::main_window)
{

	ui->setupUi(this);

	connect(ui->action_open_settings, &QAction::triggered, this, [this] {
		settings_dialog* settings = new settings_dialog(this);
		settings->setAttribute(Qt::WA_DeleteOnClose);
		settings->exec();
		});

	// Create settings manager
	settings_manager* s_manager = new settings_manager(this);

	// Create websocket client manager
	cvm::ws::client_manager* c_manager = new cvm::ws::client_manager(ui->vm_list_view);

	// Load list of servers from settings and connect to them.
	QStringList servers = s_manager->get_servers();
	for (const QString& url : servers) {
		c_manager->add_client(QUrl(url));
		c_manager->connect_client(QUrl(url));
	}

	// Setup VM list.
	cvm::models::vm_list* vm_list = new cvm::models::vm_list(ui->vm_list_view);
	connect(c_manager, &cvm::ws::client_manager::signal_list_received, vm_list, &cvm::models::vm_list::append);
	ui->vm_list_view->setModel(vm_list);

}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
}
