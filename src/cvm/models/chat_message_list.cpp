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
		return m_chat_message_list.size();
	}

	QVariant chat_message_list::data(const QModelIndex& index, int role) const
	{
		Q_ASSERT(index.isValid());
		Q_ASSERT(index.row() <= m_chat_message_list.size());

		/*switch (role)
		{

		case rank_icon_role:
		{
			user u = m_user_list.at(index.row());
			return QIcon(QString(":/images/res/rank_icons/%1.png").arg(u.m_rank));
		}


		case country_icon_role:
		{
			user u = m_user_list.at(index.row());
			return QIcon(QString(":/images/res/country_icons/%1.png").arg(u.m_country_code.toLower()));
		}


		case Qt::DisplayRole:
		{
			user u = m_user_list.at(index.row());
			return u.m_username;
		}

		case Qt::ToolTipRole:
		{
			user u = m_user_list.at(index.row());
			return QString("Rank: %1").arg(u.m_rank);
		}

		case Qt::TextAlignmentRole:
		{
			return Qt::AlignLeft;
		}

		case Qt::FontRole:
		{
			user u = m_user_list.at(index.row());
			if (u.m_rank != user::rank::unregistered)
			{
				QFont font;
				font.setBold(true);
				return font;
			}
		}

		}*/

		return {};
	}

	std::tuple<QString, QString, QString> chat_message_list::message_at_index(const QModelIndex& index) const
	{
		return m_chat_message_list.at(index.row());
	}

	void chat_message_list::append(const QString& username, const QString& message, const QUrl& server)
	{
		int row = 0;
		while (row < m_chat_message_list.count())
			++row;
		beginInsertRows(QModelIndex(), row, row);
		m_chat_message_list.insert(row, { username, message, server.url() });
		endInsertRows();
	}

	void chat_message_list::remove_row(int row)
	{
		if (row < 0 || row >= m_chat_message_list.count())
			return;

		beginRemoveRows(QModelIndex(), row, row);
		m_chat_message_list.removeAt(row);
		endRemoveRows();
	}

	void chat_message_list::clear()
	{
		if (m_chat_message_list.isEmpty())
			return;

		beginResetModel();
		m_chat_message_list.clear();
		endResetModel();

		m_chat_message_list.squeeze();
	}
}

