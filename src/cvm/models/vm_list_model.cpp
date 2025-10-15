#include "vm_list_model.h"


vm_list_model::vm_list_model(QObject* parent)
    : QAbstractListModel(parent)
{
}

int vm_list_model::rowCount(const QModelIndex&) const
{
    return vm_list.size();
}

QVariant vm_list_model::data(const QModelIndex& index, int role) const
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() <= vm_list.size());

    if (role == Qt::DisplayRole) {
	    cvm::vm vm = vm_list.at(index.row());
        return vm.m_display_name;
    }

    return {};
}

cvm::vm vm_list_model::vm(const QModelIndex& index) const
{
    return vm_list.at(index.row());
}