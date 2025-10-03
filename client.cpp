
#include "client.h"

#include "guac.h"
#include "hello_imgui/hello_imgui_logger.h"

#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXWebSocketTransport.h"

// https://medium.com/@ryan_forrester_/using-switch-statements-with-strings-in-c-a-complete-guide-efa12f64a59d :puke:
inline cvm::guac_msg_type get_guac_msg_type(const std::string& str) {
	if (str == "adduser") return cvm::guac_msg_type::adduser;
	if (str == "remuser") return cvm::guac_msg_type::remuser;
	return cvm::guac_msg_type::unknown;
}

inline void handle_guac_msg(std::string msg)
{
	std::vector<std::string> decoded_msg = guac_decode(msg);

	switch (get_guac_msg_type(decoded_msg[0]))
	{
	case cvm::guac_msg_type::adduser: // Handle user joining the vm.

		for (int i = 2; i < decoded_msg.size(); i += 2)
		{
			cvm::user new_user;

			new_user.username = decoded_msg[i];
			new_user.rank_ = static_cast<cvm::user_rank>(std::stoi(decoded_msg[i + 1]));

			client::globals::users.push_back(new_user);

			HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: User Joined: \"%s\"", new_user.username.c_str());
		}

		break;
	case cvm::guac_msg_type::remuser: // Handle user leaving the vm.

		for (int i = 2; i < decoded_msg.size(); ++i)
		{
			std::string username = decoded_msg[i];

			std::erase_if(client::globals::users, [&](const cvm::user& u) { return u.username == username; });

			HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: User Left: \"%s\"", username.c_str());
		}

		break;
	case cvm::guac_msg_type::unknown:
		break;
	}

}

static ix::WebSocket g_web_socket;

void client::init_ws_handler()
{
	HelloImGui::Log(HelloImGui::LogLevel::Info, "Initialized WS handler.");

	// Specify url to connect to

	g_web_socket.setUrl(globals::url);

	//Set origin header and add guacamole as subprotocol.
	ix::WebSocketHttpHeaders headers;
	headers["Origin"] = "https://computernewb.com/";
	g_web_socket.setExtraHeaders(headers);

	g_web_socket.addSubProtocol("guacamole");

	// Designate 3.nop as the message cvm server uses to ping the client.
	g_web_socket.setPingMessage("3.nop;", ix::SendMessageKind::Text);
	g_web_socket.setPingInterval(10);



	// Setup a callback to be fired (in a background thread, watch out for race conditions !)
	// when a message or an event (open, close, error) is received
	g_web_socket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type)
			{
			case ix::WebSocketMessageType::Ping:
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: Ping: \"%s\"", msg->str.c_str());
				break;
			case ix::WebSocketMessageType::Pong:
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: Pong: \"%s\"", msg->str.c_str());
				break;
			case ix::WebSocketMessageType::Message:
				//HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: MSG: \"%s\"", msg->str.c_str());
				handle_guac_msg(msg->str);
				break;
			case ix::WebSocketMessageType::Open:
				HelloImGui::Log(HelloImGui::LogLevel::Info, "WS: Connection Established!");
				break;
			case ix::WebSocketMessageType::Close:
				HelloImGui::Log(HelloImGui::LogLevel::Info, "WS: Connection Closed!");
				break;
			case ix::WebSocketMessageType::Fragment:
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: Fragmented MSG: \"%s\"", msg->str.c_str());
				break;
			case ix::WebSocketMessageType::Error:
				HelloImGui::Log(HelloImGui::LogLevel::Error, "WS: Error MSG: \"%s\"", msg->errorInfo.reason.c_str());
				break;
			}
		}
	);

	ui::globals::activate_ws_disable = !ui::globals::activate_ws_disable;
	ui::globals::init_ws_test_disable = !ui::globals::init_ws_test_disable;
}

void client::start_ws()
{
	g_web_socket.start();
	HelloImGui::Log(HelloImGui::LogLevel::Info, "Connecting to \"%s\"", globals::url);
	ui::globals::activate_ws_disable = !ui::globals::activate_ws_disable;
	ui::globals::deactivate_ws_disable = !ui::globals::deactivate_ws_disable;
}

void client::stop_ws()
{
	g_web_socket.stop();

	//clear user list, etc
	globals::users.clear();
	globals::users.shrink_to_fit();

	ui::globals::activate_ws_disable = !ui::globals::activate_ws_disable;
	ui::globals::deactivate_ws_disable = !ui::globals::deactivate_ws_disable;
}
