// CollabVM related structs, etc
#pragma once
#include <qpixmap.h>
#include <qstring.h>
#include <QWebSocket>

namespace cvm
{

	//TODO: Should i make any of these members private or keep them public? I don't know.

	struct user
	{
		enum rank : qint8 {
			unregistered,
			registered,
			admin,
			moderator
		};

		QString m_username;

		rank m_rank;

		int m_turn = 0;

		QString m_country_code;

		user(const QString& username, const rank& rank);
	};

	struct vm
	{
		QString m_id;

		QString m_display_name; // Name of the VM, can contain HTML

		QPixmap m_thumbnail; // Image is decoded from base64.

		vm(const QString& id, const QString& display_name, const QString& thumbnail);
	};

	struct chat_message
	{
		user* sender;

		QString message;
	};

	struct client
	{
		QUrl m_url;

		QList<vm> m_vms;
		QList<user> m_users;

		QWebSocket* m_socket;
		bool m_is_connected;

		client(const QUrl& name);
	};
}
