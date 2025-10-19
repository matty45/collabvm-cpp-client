#include "vm_window.h"

#include "ui_vm_window.h"

vm_window::vm_window(const cvm::vm& vm, QWidget* parent)
    : QWidget(parent), ui(new Ui::vm_window)
{

    ui->setupUi(this);

}

vm_window::~vm_window()
{
}