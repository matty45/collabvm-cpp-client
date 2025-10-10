// CollabVM related structs, etc
#pragma once
#include <qstring.h>

namespace cvm
{
	enum user_rank {
		Unregistered = 0,
		Registered = 1,
		Admin = 2,
		Moderator = 3
	};

	struct user
	{
		QString username;

		user_rank rank_ = Unregistered;

		int turn_ = 0;

		QString country_code;
	};

	struct vm
	{
		QString id;

		QString display_name;

		QString thumbnail; // Base64 encoded image png/jpeg (Will need to check the magic programatically)
	};

	enum guac_msg_type {
		auth,
		cap,
		list,
		adduser,
		remuser,
		chat,
		rename,
		flag,
		unknown
	};
}
