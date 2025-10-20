// CollabVM related structs, etc
#pragma once
#include <qpixmap.h>
#include <qstring.h>
#include <QUrl>

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

		QUrl m_server; // Websocket server url that was used to connect to this vm.

		vm(const QString& id, const QString& display_name, const QString& thumbnail, const QUrl& server);
	};
}
