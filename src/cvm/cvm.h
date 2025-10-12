// CollabVM related structs, etc
#pragma once
#include <qstring.h>

namespace cvm
{
	class user
	{
	public:
		enum rank : int8_t {
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

	class vm
	{
		QString m_id;

		QString m_display_name; // Name of the VM, can contain HTML

		QString m_thumbnail; // Base64 encoded image png/jpeg (Will need to check the magic programatically)

	public:
		vm(const QString& id, const QString& display_name, const QString& thumbnail);
	};

}
