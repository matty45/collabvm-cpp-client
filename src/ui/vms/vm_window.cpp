#include "vm_window.h"

#include "ui_vm_window.h"
#include "src/cvm/models/proxies/user_filter_proxy.h"

vm_window::vm_window(const cvm::vm& vm, QAbstractListModel* user_list_model, QWidget* parent)
    : QWidget(parent), m_ui(new Ui::vm_window)
{
    m_ui->setupUi(this);

	// Set filter
	m_user_filter_proxy = new cvm::models::proxies::user_filter_proxy(this);
    m_user_filter_proxy->set_filter_server(vm.m_server);
    m_user_filter_proxy->setSourceModel(user_list_model);

    m_ui->user_list_view->setModel(m_user_filter_proxy);
}

vm_window::~vm_window()
{
}

