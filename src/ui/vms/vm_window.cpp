#include "vm_window.h"

#include "ui_vm_window.h"

vm_window::vm_window(const cvm::vm& vm, QWidget* parent)
    : QWidget(parent), m_ui(new Ui::vm_window)
{

    m_ui->setupUi(this);

}

vm_window::~vm_window()
{
}