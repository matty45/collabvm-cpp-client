#pragma once
#include <string>

#include "ixwebsocket/IXWebSocket.h"

inline const std::string g_window_title = "CollabVM Experimental C++ Client";

inline bool activate_ws_test_disable = true;

inline bool deactivate_ws_test_disable = true;

inline bool init_ws_test_disable = false;

inline char ws_url[255] = "wss://computernewb.com/collab-vm/vm1";