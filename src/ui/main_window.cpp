#include "main_window.h"

#include "settings_dialog.h"
#include "ui_main_window.h"
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

	connect(ui->action_debug_button, &QAction::triggered, s_manager, [s_manager] {
		QStringList test = {
		"wss://computernewb.com/collab-vm/vm1",
		"wss://computernewb.com/collab-vm/vm2",
		"wss://computernewb.com/collab-vm/vm3",
		"wss://computernewb.com/collab-vm/vm4",
		"wss://computernewb.com/collab-vm/vm5",
		"wss://computernewb.com/collab-vm/vm6",
		"wss://computernewb.com/collab-vm/vm7",
		"wss://computernewb.com/collab-vm/vm8",
		"wss://computernewb.com/collab-vm/vm9"
		};

		s_manager->save_servers(test);
		});

	// Create websocket client manager
	cvm::ws::client_manager* c_manager = new cvm::ws::client_manager(ui->vm_list_view);

	// Load list of servers from settings and connect to them.
	QStringList servers = s_manager->get_servers();
	for (const QString& url : servers) {
		c_manager->add_client(QUrl(url));
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
