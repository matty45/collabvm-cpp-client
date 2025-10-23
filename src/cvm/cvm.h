// CollabVM related structs, etc
#pragma once
#include <qpixmap.h>
#include <qstring.h>

namespace cvm
{
	class server;

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

		server* m_server; // Pointer to parent server  

		user(const QString& username, const rank& rank, server* server);
	};

	struct vm
	{
		QString m_id;

		QString m_display_name; // Name of the VM, can contain HTML

		QPixmap m_thumbnail; // Image is decoded from base64.

		server* m_server; // Pointer to parent server  

		vm(const QString& id, const QString& display_name, const QString& thumbnail, server* server);
	};

	struct chat_message
	{
		user* sender;

		QString message;

		server* m_server; // Pointer to parent server  
	};
}
