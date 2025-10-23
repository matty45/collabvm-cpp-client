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

		user* u = m_user_list.at(index.row());

		switch (role)
		{

		case rank_icon_role:
		{
			
			return QIcon(QString(":/images/res/rank_icons/%1.png").arg(u->m_rank));
		}


		case country_icon_role:
		{
			return QIcon(QString(":/images/res/country_icons/%1.png").arg(u->m_country_code.toLower()));
		}


		case Qt::DisplayRole:
		{
			return u->m_username;
		}

		case Qt::ToolTipRole:
		{
			return QString("Rank: %1 Country Code: %2").arg(u->m_rank).arg(u->m_country_code);
		}

		case Qt::TextAlignmentRole:
		{
			return Qt::AlignLeft;
		}

		case Qt::FontRole:
		{
			if (u->m_rank != user::rank::unregistered)
			{
				QFont font;
				font.setBold(true);
				return font;
			}
		}

		}

		return {};
	}

	user* user_list::user_at_index(const QModelIndex& index) const
	{
		return m_user_list.at(index.row());
	}

	user* user_list::find_user_by_username(const QString& username) const
	{
		for (int i = 0; i < m_user_list.count(); ++i)
		{
			if (m_user_list.at(i)->m_username == username)
			{
				return m_user_list.at(i);
			}
		}
		return nullptr;
	}

	void user_list::append(user* user)
	{
		if (!user)
			return;


		int row = 0;
		while (row < m_user_list.count() && user->m_username > m_user_list.at(row)->m_username)
			++row;
		beginInsertRows(QModelIndex(), row, row);
		m_user_list.insert(row, user);
		endInsertRows();
	}

	void user_list::remove(const user* marked_for_death)
	{
		for (int i = 0; i < m_user_list.size(); ++i) {
			if (m_user_list[i] == marked_for_death) {
				beginRemoveRows(QModelIndex(), i, i);
				m_user_list.removeAt(i);
				endRemoveRows();
				break;
			}
		}
	}

	void user_list::clear()
	{
		if (m_user_list.isEmpty())
			return;

		qDeleteAll(m_user_list);

		beginResetModel();
		m_user_list.clear();
		endResetModel();

		m_user_list.squeeze();
	}
}

