#include "vm_window.h"

#include "ui_vm_window.h"
#include "src/cvm/server.h"
#include "src/cvm/models/delegates/user.h"
#include "src/cvm/models/proxies/user_filter.h"

vm_window::vm_window(const cvm::vm* vm, bool persistence_mode, QWidget* parent)
    : QWidget(parent), m_ui(new Ui::vm_window)
{
    m_persistence_mode = persistence_mode;

    m_vm = vm;

    m_ui->setupUi(this);

	// Set filter
	m_user_filter_proxy = new cvm::models::proxies::user_filter(this);
    m_user_filter_proxy->set_filter_server(m_vm->m_server);
    m_user_filter_proxy->setSourceModel(m_vm->m_server->user_model());

    m_ui->user_list_view->setModel(m_user_filter_proxy);

    //set delegate
    cvm::delegates::user* delegate = new cvm::delegates::user(m_ui->user_list_view);
    m_ui->user_list_view->setItemDelegate(delegate);

}

vm_window::~vm_window()
{
    if (!m_persistence_mode)
    m_vm->m_server->disconnect_from_server();
}

