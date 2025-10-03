#pragma once
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
		std::string username;

		user_rank rank_ = Unregistered;

		int turn_ = 0;

		std::string country_code;
	};

	enum class guac_msg_type {
		adduser,
		list,
		nop,
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

	inline cvm::user users[];
}

namespace client
{
	void init_ws_handler();
	void start_ws();
	void stop_ws();

	// https://medium.com/@ryan_forrester_/using-switch-statements-with-strings-in-c-a-complete-guide-efa12f64a59d :puke:
	inline cvm::guac_msg_type get_guac_msg_type(const std::string& str) {
		if (str == "adduser") return cvm::guac_msg_type::adduser;
		return cvm::guac_msg_type::unknown;
	}

	inline void handle_guac_msg(std::string msg)
	{
		std::vector<std::string> decoded_msg = guac_decode(msg);

		switch (get_guac_msg_type(decoded_msg[0]))
		{
		case cvm::guac_msg_type::adduser:
			break;
		case cvm::guac_msg_type::list:
			break;
		case cvm::guac_msg_type::nop:
			break;
		case cvm::guac_msg_type::unknown:
			break;
		}

	}
}
