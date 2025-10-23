#include "vm_window.h"

#include "ui_vm_window.h"
#include "src/cvm/server.h"
#include "src/cvm/models/delegates/user.h"

vm_window::vm_window(const cvm::vm* vm, bool persistence_mode, QWidget* parent)
    : QWidget(parent), m_ui(new Ui::vm_window)
{
    m_persistence_mode = persistence_mode;

    m_vm = vm;

    m_ui->setupUi(this);

    m_ui->user_list_view->setModel(m_vm->m_server->user_model());

    //set delegate
    cvm::delegates::user* delegate = new cvm::delegates::user(m_ui->user_list_view);
    m_ui->user_list_view->setItemDelegate(delegate);

    m_ui->chat_msg_list_view->setModel(m_vm->m_server->chat_model());

}

vm_window::~vm_window()
{
    if (!m_persistence_mode)
    m_vm->m_server->disconnect_from_server();
}

