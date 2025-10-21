#include "user_list.h"

#include <qfont.h>
#include <QMessageBox>

namespace cvm::models
{
	user_list::user_list(QObject* parent)
		: QAbstractListModel(parent)
	{
	}

	int user_list::rowCount(const QModelIndex&) const
	{
		return m_user_list.size();
	}

	QVariant user_list::data(const QModelIndex& index, int role) const
	{
		Q_ASSERT(index.isValid());
		Q_ASSERT(index.row() <= m_user_list.size());


		switch (role)
		{

		case Qt::DisplayRole:
		{
			user u = m_user_list.at(index.row());
			return u.m_username;
		}

		}

		return {};
	}

	user user_list::user_at_index(const QModelIndex& index) const
	{
		return m_user_list.at(index.row());
	}

	void user_list::append(const QString& username, const user::rank& rank, const QUrl& server)
	{

		// Check for duplicates and update if found  
		for (int i = 0; i < m_user_list.count(); ++i)
		{
			if (m_user_list.at(i).m_username == username && m_user_list.at(i).m_server == server)
			{
				// Update existing entry

				qDebug() << "Users: Updating existing user entry" << username << rank << server;

				m_user_list[i] = { username, rank, server };
				emit dataChanged(index(i), index(i));
				return;
			}
		}

		qDebug() << "Users: Adding user entry" << username << rank << server;

		int row = 0;
		while (row < m_user_list.count() && username > m_user_list.at(row).m_username)
			++row;
		beginInsertRows(QModelIndex(), row, row);
		m_user_list.insert(row, { username, rank, server });
		endInsertRows();
	}

	void user_list::remove(const QString& username, const QUrl& server)
	{
		for (int i = 0; i < m_user_list.count(); ++i)
		{
			if (m_user_list.at(i).m_username == username && m_user_list.at(i).m_server == server)
			{
				beginRemoveRows(QModelIndex(), i, i);
				m_user_list.removeAt(i);
				endRemoveRows();
				return;
			}
		}
	}

	void user_list::remove_row(int row)
	{
		if (row < 0 || row >= m_user_list.count())
			return;

		beginRemoveRows(QModelIndex(), row, row);
		m_user_list.removeAt(row);
		endRemoveRows();
	}

	void user_list::clear()
	{
		if (m_user_list.isEmpty())
			return;

		beginResetModel();
		m_user_list.clear();
		endResetModel();

		m_user_list.squeeze();
	}
}

