/*
 *  main.cpp
 *  Author: Benjamin Sergeant
 *  Copyright (c) 2020 Machine Zone, Inc. All rights reserved.
 *
 *  Super simple standalone example. See ws folder, unittest and doc/usage.md for more.
 *
 *  On macOS
 *  $ mkdir -p build ; (cd build ; cmake -DUSE_TLS=1 .. ; make -j ; make install)
 *  $ clang++ --std=c++11 --stdlib=libc++ main.cpp -lixwebsocket -lz -framework Security -framework Foundation
 *  $ ./a.out
 *
 *  Or use cmake -DBUILD_DEMO=ON option for other platforms
 */

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>

int main()
{
	// Required on Windows
	ix::initNetSystem();

	// Our websocket object
	ix::WebSocket webSocket;

	// Connect to a server with encryption
	// See https://machinezone.github.io/IXWebSocket/usage/#tls-support-and-configuration
	//     https://github.com/machinezone/IXWebSocket/issues/386#issuecomment-1105235227 (self signed certificates)
	std::string url("wss://computernewb.com/collab-vm/vm1");
	webSocket.setUrl(url);

	ix::WebSocketHttpHeaders headers;
	headers["Origin"] = "https://computernewb.com/";
	webSocket.setExtraHeaders(headers);

	webSocket.addSubProtocol("guacamole");

	webSocket.setPingMessage("3.nop;",ix::SendMessageKind::Text);
	webSocket.setPingInterval(10);

	std::cout << "Connecting to " << url << "..." << '\n';

	// Setup a callback to be fired (in a background thread, watch out for race conditions !)
	// when a message or an event (open, close, error) is received
	webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type)
			{
			case ix::WebSocketMessageType::Ping:
				std::cout << "ping data: " << msg->str << '\n';
				break;
			case ix::WebSocketMessageType::Pong:
				std::cout << "pong data: " << msg->str << '\n';
				break;
			case ix::WebSocketMessageType::Message:
				std::cout << "message data: " << msg->str << '\n';
				break;
			case ix::WebSocketMessageType::Open:
				std::cout << "Connection established" << '\n';
				break;
			case ix::WebSocketMessageType::Error:
				std::cout << "Connection error: " << msg->errorInfo.reason << '\n';
				break;
			default:
				std::cout << "Unknown Websocket Message: " << msg->str << '\n';
				break;
			}
			std::cout << "> " << std::flush;
		}
	);

	// Now that our callback is setup, we can start our background thread and receive messages
	webSocket.start();

	// Send a message to the server (default to TEXT mode)
	//webSocket.send("hello world");

	// Display a prompt
	std::cout << "> " << std::flush;

	std::string text;
	// Read text from the console and send messages in text mode.
	// Exit with Ctrl-D on Unix or Ctrl-Z on Windows.
	while (std::getline(std::cin, text))
	{
		webSocket.send(text);
		std::cout << "> " << std::flush;
	}

	return 0;
}