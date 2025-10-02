
#include "client.h"
#include <iostream>

#include "globals.h"
#include "hello_imgui/hello_imgui_logger.h"

#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXWebSocketTransport.h"

//void handle_cvm_guac_msg(const ix::WebSocketMessagePtr& msg)
//{
//
//}

ix::WebSocket g_web_socket;

void ws_test()
{
	HelloImGui::Log(HelloImGui::LogLevel::Debug, "Activated WS test.");

	// Specify url to connect to
	const std::string url("wss://computernewb.com/collab-vm/vm7");
	g_web_socket.setUrl(url);

	//Set origin header and add guacamole as subprotocol.
	ix::WebSocketHttpHeaders headers;
	headers["Origin"] = "https://computernewb.com/";
	g_web_socket.setExtraHeaders(headers);

	g_web_socket.addSubProtocol("guacamole");

	// Designate 3.nop as the message cvm server uses to ping the client.
	g_web_socket.setPingMessage("3.nop;", ix::SendMessageKind::Text);
	g_web_socket.setPingInterval(10);

	HelloImGui::Log(HelloImGui::LogLevel::Debug, "Connecting to \"%s\"",url.c_str());

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
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: MSG: \"%s\"", msg->str.c_str());
				//handle_cvm_guac_msg(msg)
				break;
			case ix::WebSocketMessageType::Open:
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: Connection Established!");
				break;
			case ix::WebSocketMessageType::Close:
				HelloImGui::Log(HelloImGui::LogLevel::Debug, "WS: Connection Closed!");
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

	g_web_socket.start();
}
