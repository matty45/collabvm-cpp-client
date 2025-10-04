#pragma once
#include <algorithm>
#include <string>

#include "guac.h"
#include "ixwebsocket/IXWebSocket.h"

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
		chat,
		rename,
		flag,
		unknown
	};
}


namespace ui
{
	inline const std::string g_window_title = "CollabVM Experimental C++ Client";

	inline bool g_activate_ws_disable = true;

	inline bool g_deactivate_ws_disable = true;

	inline bool g_init_ws_test_disable = false;
}


namespace client
{
	/**
 * The URL of the websocket server to connect to.
 */
	inline char g_url[255] = "wss://computernewb.com/collab-vm/vm9";

	inline ix::WebSocket g_web_socket;

	inline std::vector<cvm::user> g_user_roster;

	void init_ws_handler();
	void start_ws();
	void stop_ws();
}
