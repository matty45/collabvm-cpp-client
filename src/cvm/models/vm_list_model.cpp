#include "vm_list_model.h"


vm_list_model::vm_list_model(QObject* parent)
    : QAbstractListModel(parent)
{
}

int vm_list_model::rowCount(const QModelIndex&) const
{
    return m_vm_list.size();
}

QVariant vm_list_model::data(const QModelIndex& index, int role) const
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() <= m_vm_list.size());

    if (role == Qt::DecorationRole)
    {
        cvm::vm vm = m_vm_list.at(index.row());
        return vm.m_thumbnail;
    }
    else if (role == Qt::DisplayRole) {
	    cvm::vm vm = m_vm_list.at(index.row());
        return vm.m_display_name;
    }

    return {};
}

cvm::vm vm_list_model::vm(const QModelIndex& index) const
{
    return m_vm_list.at(index.row());
}

void vm_list_model::append(const QString& id, const QString& display_name, const QString& thumbnail)
{
    int row = 0;
    while (row < m_vm_list.count() && id > m_vm_list.at(row).m_id)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_vm_list.insert(row, { id, display_name, thumbnail });
    endInsertRows();
}

void vm_list_model::remove(int row)
{
    if (row < 0 || row >= m_vm_list.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_vm_list.removeAt(row);
    endRemoveRows();
}