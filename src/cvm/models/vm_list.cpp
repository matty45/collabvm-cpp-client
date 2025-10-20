#include "vm_list.h"

#include <qfont.h>

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


		switch (role)
		{
		case Qt::DecorationRole:
		{
			cvm::vm vm = m_vm_list.at(index.row());
			return vm.m_thumbnail;
		}

		case Qt::DisplayRole:
		{
			cvm::vm vm = m_vm_list.at(index.row());
			return vm.m_display_name;
		}

		case Qt::FontRole:
		{
			QFont font;
			font.setBold(true);
			return font;
		}

		}

		return {};
	}

	vm vm_list::vm(const QModelIndex& index) const
	{
		return m_vm_list.at(index.row());
	}

	void vm_list::append(const QString& id, const QString& display_name, const QString& thumbnail, const QUrl& server)
	{
		int row = 0;
		while (row < m_vm_list.count() && id > m_vm_list.at(row).m_id)
			++row;
		beginInsertRows(QModelIndex(), row, row);
		m_vm_list.insert(row, { id, display_name, thumbnail, server });
		endInsertRows();
	}

	void vm_list::remove(int row)
	{
		if (row < 0 || row >= m_vm_list.count())
			return;

		beginRemoveRows(QModelIndex(), row, row);
		m_vm_list.removeAt(row);
		endRemoveRows();
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

