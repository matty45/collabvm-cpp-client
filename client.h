#pragma once
#include <algorithm>
#include <string>

#include "guac.h"

//TODO: Split this all off into separate header files later when things get too big.


// CVM related structs, etc

namespace cvm
{
	enum user_rank {
		Unregistered = 0,
		Registered = 1,
		Admin = 2,
		Moderator = 3
	};

	class user
	{
	public:
		std::string username;

		user_rank rank_ = Unregistered;

		int turn_ = 0;

		std::string country_code;
	};

	enum class guac_msg_type {
		adduser,
		remuser,
		unknown
	};
}


namespace ui::globals
{
	inline const std::string window_title = "CollabVM Experimental C++ Client";

	inline bool activate_ws_disable = true;

	inline bool deactivate_ws_disable = true;

	inline bool init_ws_test_disable = false;
}

namespace client::globals
{
	/**
	 * The URL of the websocket server to connect to.
	 */
	inline char url[255] = "wss://computernewb.com/collab-vm/vm9";

	inline std::vector<cvm::user> users;
}

namespace client
{
	void init_ws_handler();
	void start_ws();
	void stop_ws();
}
