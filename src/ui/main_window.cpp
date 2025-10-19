#include "main_window.h"

#include "ui_main_window.h"
#include "settings/settings_dialog.h"
#include "src/cvm/ws/ws_manager.h"
#include "src/settings/settings_manager.h"
#include "vms/vm_window.h"

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
	cvm::ws::client_manager* c_manager = new cvm::ws::client_manager(this);

	// Load list of servers from settings and connect to them.
	QStringList servers = s_manager->get_servers();
	for (const QString& url : servers) {
		c_manager->add_client(QUrl(url));
		c_manager->connect_client(QUrl(url));
	}

	// Setup VM list.
	cvm::models::vm_list* vm_list = new cvm::models::vm_list(this);
	connect(c_manager, &cvm::ws::client_manager::signal_list_received, vm_list, &cvm::models::vm_list::append);
	ui->vm_list_view->setModel(vm_list);

	connect(ui->vm_list_view, &QAbstractItemView::doubleClicked, this, &main_window::on_vm_double_clicked);

}

void main_window::on_vm_double_clicked(const QModelIndex& index) {
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
    vm_window* vm_w = new vm_window(vm_data, this);
    m_open_vm_windows[vm_data.m_id] = vm_w;

    // Clean up when window closes  
    connect(vm_w, &QWidget::destroyed, this, [this, vm_id = vm_data.m_id]() {
        m_open_vm_windows.remove(vm_id);
        });

    vm_w->show();
	vm_w->setWindowTitle(vm_data.m_display_name);
}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
}
