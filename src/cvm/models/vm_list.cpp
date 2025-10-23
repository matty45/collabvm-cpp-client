#include "vm_list.h"

#include <qfont.h>
#include <QMessageBox>

#include "src/cvm/server.h"

namespace cvm::models
{
	vm_list::vm_list(QObject* parent)
		: QAbstractListModel(parent)
	{
	}

	int vm_list::rowCount(const QModelIndex&) const
	{
		return m_vm_list.size();
	}

	QVariant vm_list::data(const QModelIndex& index, int role) const
	{
		Q_ASSERT(index.isValid());
		Q_ASSERT(index.row() <= m_vm_list.size());

		vm* current_vm = m_vm_list.at(index.row());

		switch (role)
		{
		case Qt::DecorationRole:
		{
			return current_vm->m_thumbnail;
		}

		case Qt::DisplayRole:
		{
			return current_vm->m_display_name;
		}

		case Qt::ToolTipRole:
		{
			// Show server information in tooltip  
			QString tooltip = QString("<b>ID:</b> %1<br>").arg(current_vm->m_id);
			tooltip += QString("<b>URL:</b> %1<br>").arg(current_vm->m_server->url().toString());
			tooltip += QString("<b>Status:</b> %1<br>").arg(current_vm->m_server->is_connected() ? "Connected" : "Disconnected");
			tooltip += QString("<b>Users online:</b> %1<br>").arg(current_vm->m_server->user_count());

			return tooltip;
		}

		}

		return {};
	}

	vm* vm_list::vm_at_index(const QModelIndex& index) const
	{
		return m_vm_list.at(index.row());
	}

	void vm_list::append(vm* vm)
	{

		if (!vm)
			return;


		int row = 0;
		while (row < m_vm_list.count() && vm->m_id > m_vm_list.at(row)->m_id)
			++row;
		beginInsertRows(QModelIndex(), row, row);
		m_vm_list.insert(row, vm);
		endInsertRows();
	}

	void vm_list::remove(const vm* marked_for_death)
	{
		for (int i = 0; i < m_vm_list.size(); ++i) {
			if (m_vm_list[i] == marked_for_death) {
				beginRemoveRows(QModelIndex(), i, i);
				m_vm_list.removeAt(i);
				endRemoveRows();
				break;
			}
		}
	}

	void vm_list::clear()
	{
		if (m_vm_list.isEmpty())
			return;

		beginResetModel();
		m_vm_list.clear();
		endResetModel();

		m_vm_list.squeeze();
	}
}

