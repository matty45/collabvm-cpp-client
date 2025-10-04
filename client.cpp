
#include "client.h"

#include <algorithm>

#include "guac.h"
#include "hello_imgui/hello_imgui_logger.h"
#include "ixwebsocket/IXHttpServer.h"

#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXWebSocketTransport.h"

// https://medium.com/@ryan_forrester_/using-switch-statements-with-strings-in-c-a-complete-guide-efa12f64a59d :puke:
inline cvm::guac_msg_type get_guac_msg_type(const std::string& str) {

	// User manipulation
	if (str == "adduser") return cvm::guac_msg_type::adduser;
	if (str == "remuser") return cvm::guac_msg_type::remuser;
	if (str == "rename") return cvm::guac_msg_type::rename;
	if (str == "flag") return cvm::guac_msg_type::flag;

	// Client stuff
	if (str == "chat") return cvm::guac_msg_type::chat;
	if (str == "auth") return cvm::guac_msg_type::auth;
	if (str == "list") return cvm::guac_msg_type::list;
	return cvm::guac_msg_type::unknown;
}

// ReSharper disable once CppNotAllPathsReturnValue
// This should never cause an error unless i am doing something really stupid.
inline std::ranges::borrowed_iterator_t<std::vector<cvm::user>&> find_user_by_name(std::string username)
{
	std::ranges::borrowed_iterator_t<std::vector<cvm::user>&> i = std::ranges::find(client::g_user_roster, username, &cvm::user::username);
	if (i != client::g_user_roster.end())
		return i;
}

// TODO: Probably move these handling functions into their own file or something.
inline void handle_adduser(std::vector<std::string> decoded_msg)
{
	for (int i = 2; i < decoded_msg.size(); i += 2)
	{
		cvm::user new_user;

		new_user.username = decoded_msg[i];
		new_user.rank_ = static_cast<cvm::user_rank>(std::stoi(decoded_msg[i + 1]));

		client::g_user_roster.push_back(new_user);

		HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: User Joined: \"%s\"", new_user.username.c_str());
	}
}

inline void handle_remuser(std::vector<std::string> decoded_msg)
{
	for (int i = 2; i < decoded_msg.size(); ++i)
	{
		// wish there was a one-liner equivalent for finding a member of a vector instead of deleting it but all we have is find_if or std::ranges::find :(
		std::erase_if(client::g_user_roster, [&](const cvm::user& u) { return u.username == decoded_msg[i]; }); 

		HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: User Left: \"%s\"", decoded_msg[i].c_str());
	}
}

inline void handle_rename(std::vector<std::string> decoded_msg)
{
	if (std::stoi(decoded_msg[1]) == 0) // 0 if this is the client that requested a rename, 1 if if anyone else.
	{
		HelloImGui::Log(HelloImGui::LogLevel::Error, "CVM: Client renaming not implemented yet.");
		return;
	}

	std::ranges::borrowed_iterator_t<std::vector<cvm::user>&> user = find_user_by_name(decoded_msg[2]);

	user->username = decoded_msg[3];
	HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: Renamed User \"%s\" to \"%s\"", decoded_msg[2].c_str(), user->username.c_str());

}

inline void handle_chat(std::vector<std::string> decoded_msg)
{
	for (int i = 1; i < decoded_msg.size(); i += 2)
		HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: Chat: %s : %s", decoded_msg[i].c_str(), decoded_msg[i + 1].c_str());
}

inline void handle_flag(const std::vector<std::string>& decoded_msg)
{
	for (int i = 1; i < decoded_msg.size(); i += 2)
	{
		std::ranges::borrowed_iterator_t<std::vector<cvm::user>&> user = find_user_by_name(decoded_msg[i]);

		user->country_code = decoded_msg[i + 1];

		HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: Added country code \"%s\" to User \"%s\"", user->country_code.c_str(),user->username.c_str());
	}
}

inline void handle_list(std::vector<std::string> decoded_msg)
{
	for (int i = 1; i < decoded_msg.size(); i += 3)
	{
		cvm::vm new_vm;

		new_vm.id = decoded_msg[i];
		new_vm.display_name = decoded_msg[i + 1];
		new_vm.thumbnail = decoded_msg[i + 2];

		client::g_vm_list.push_back(new_vm);

		HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: VM added: \"%s\"", new_vm.id.c_str());
	}
}

inline void handle_guac_msg(std::string msg)
{
	std::vector<std::string> decoded_msg = guac_decode(msg);

	switch (get_guac_msg_type(decoded_msg[0]))
	{
	case cvm::guac_msg_type::adduser: // Handle user joining the vm.
		handle_adduser(decoded_msg);
		break;
	case cvm::guac_msg_type::remuser: // Handle user leaving the vm.
		handle_remuser(decoded_msg);
		break;
	case cvm::guac_msg_type::rename: // Handle renaming user.
		handle_rename(decoded_msg);
		break;
	case cvm::guac_msg_type::chat: // Handle user chat message.
		handle_chat(decoded_msg);
		break;
	case cvm::guac_msg_type::flag: // Handle user country code/flag/whatever.
		handle_flag(decoded_msg);
		break;
	case cvm::guac_msg_type::auth:
		server::g_server_has_cvm_auth = true;
		HelloImGui::Log(HelloImGui::LogLevel::Info, "CVM: Server is using the CollabVM Account Auth System!");
		break;
	case cvm::guac_msg_type::list:
		handle_list(decoded_msg);
		break;
	case cvm::guac_msg_type::unknown:
		break;
	}
}


void client::init_ws_handler()
{
	HelloImGui::Log(HelloImGui::LogLevel::Info, "Initialized WS handler.");

	// Specify url to connect to

	g_web_socket.setUrl(g_url);

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

				// Get list of VMS!
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "CVM: Sending list ws message.");
				g_web_socket.send(guac_encode({ "list" }));

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

	ui::g_activate_ws_disable = !ui::g_activate_ws_disable;
	ui::g_init_ws_test_disable = !ui::g_init_ws_test_disable;
}

void client::start_ws()
{
	g_web_socket.start();
	HelloImGui::Log(HelloImGui::LogLevel::Info, "Connecting to \"%s\"", g_url);
	ui::g_activate_ws_disable = !ui::g_activate_ws_disable;
	ui::g_deactivate_ws_disable = !ui::g_deactivate_ws_disable;
}

void client::stop_ws()
{
	g_web_socket.stop();

	//clear user list, etc
	g_user_roster.clear();
	g_user_roster.shrink_to_fit();

	g_vm_list.clear();
	g_vm_list.shrink_to_fit();

	server::g_server_has_cvm_auth = false; //TODO: Should probably make a class for this stuff but i don't know how to implement that in a tidy way. My c++ knowledge has faded over the months.

	ui::g_activate_ws_disable = !ui::g_activate_ws_disable;
	ui::g_deactivate_ws_disable = !ui::g_deactivate_ws_disable;
}
