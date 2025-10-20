#include "vm_list.h"

#include <qfont.h>
#include <QMessageBox>

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
			vm vm = m_vm_list.at(index.row());
			return vm.m_thumbnail;
		}

		case Qt::DisplayRole:
		{
			vm vm = m_vm_list.at(index.row());
			return vm.m_display_name;
		}

		case Qt::ToolTipRole:
		{
			vm vm = m_vm_list.at(index.row());

			return QString("Server: %1").arg(vm.m_server.url());
		}

		}

		return {};
	}

	vm vm_list::vm_at_index(const QModelIndex& index) const
	{
		return m_vm_list.at(index.row());
	}


	void vm_list::append_user(const QString& username, const user::rank& rank, const QUrl& server)
	{
		bool found_vm = false;
		// Find vm 
		for (int i = 0; i < m_vm_list.count(); ++i)
		{
			if (m_vm_list.at(i).m_server == server)
			{
				vm veem = m_vm_list[i];

				// Update existing entry  
				veem.m_users.append({ username, rank });
				emit dataChanged(index(i), index(i));
				found_vm = true;
			}
		}

		if (!found_vm)
		{
			QMessageBox::critical(
				nullptr,
				"VM User Addition Error!",
				QString("Could not add user to VM as it possibly does not exist. : %2").arg(server.url()),
				QMessageBox::Ok
			);
		}

	}

	void vm_list::append(const QString& id, const QString& display_name, const QString& thumbnail, const QUrl& server)
	{

		// Check for duplicates and update if found  
		for (int i = 0; i < m_vm_list.count(); ++i)
		{
			if (m_vm_list.at(i).m_server == server)
			{
				// Update existing entry  
				m_vm_list[i] = { id, display_name, thumbnail,server };
				emit dataChanged(index(i), index(i));
				return;
			}
		}

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

