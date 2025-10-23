#include "chat_message_list.h"

#include <qfont.h>
#include <QMessageBox>

namespace cvm::models
{
	chat_message_list::chat_message_list(QObject* parent)
		: QAbstractListModel(parent)
	{
	}

	int chat_message_list::rowCount(const QModelIndex&) const
	{
		return m_chat_message_qlist.size();
	}

	QVariant chat_message_list::data(const QModelIndex& index, int role) const
	{
		Q_ASSERT(index.isValid());
		Q_ASSERT(index.row() <= m_chat_message_qlist.size());


		chat_message* msg = m_chat_message_qlist.at(index.row());

		switch (role)
		{

		case Qt::DisplayRole:
		{
			return QString("%1 : %2").arg(msg->sender).arg(msg->message);
		}

		}

		return {};
	}

	chat_message* chat_message_list::message_at_index(const QModelIndex& index) const
	{
		return m_chat_message_qlist.at(index.row());
	}

	void chat_message_list::append(chat_message* message)
	{
		int row = 0;
		while (row < m_chat_message_qlist.count())
			++row;
		beginInsertRows(QModelIndex(), row, row);
		m_chat_message_qlist.insert(row, message);
		endInsertRows();
	}

	void chat_message_list::remove_row(int row)
	{
		if (row < 0 || row >= m_chat_message_qlist.count())
			return;

		beginRemoveRows(QModelIndex(), row, row);
		m_chat_message_qlist.removeAt(row);
		endRemoveRows();
	}

	void chat_message_list::clear()
	{
		if (m_chat_message_qlist.isEmpty())
			return;

		beginResetModel();
		m_chat_message_qlist.clear();
		endResetModel();

		m_chat_message_qlist.squeeze();
	}
}

