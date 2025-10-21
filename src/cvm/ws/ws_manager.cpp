#include "ws_manager.h"
#include "src/guac.h"
#include "QMessageBox"

namespace cvm::ws
{
	client_manager::client_manager(QObject* parent) :
		QObject(parent), m_persistence_mode(false)
	{
	}

	void client_manager::clear_all_clients()
	{
		if (m_clients.isEmpty()) {
			emit all_clients_cleared();
			return;
		}

		m_pending_disconnections = m_clients.size();

		// Close all clients - this triggers disconnected() signal for each  
		for (QWebSocket* socket : m_clients) {
			if (socket) {
				socket->close();
			}
		}
	}

	client_manager::~client_manager()
	{
		qDebug() << "Shutting down client manager!";

		clear_all_clients();
	}

	QWebSocket* client_manager::find_client_by_url(const QUrl& url) const
	{
		for (QWebSocket* client : m_clients)
			if (client->requestUrl() == url)
				return client;

		return nullptr;
	}

	// Researched how KDE manages multiple websockets. https://github.com/KDE/tokodon/blob/master/src/account/account.h 
	void client_manager::add_client(const QUrl& url)
	{

		if (url.scheme() != "wss" && url.scheme() != "ws")
		{

			QMessageBox::critical(
				nullptr,
				"Websocket client creation error",
				QString("Invalid URL: %1 does not have wss:// or ws:// as its prefix!").arg(url.url()),
				QMessageBox::Ok
			);

			return;
		}

		if (find_client_by_url(url))
		{
			QMessageBox::critical(
				nullptr,
				"Websocket client creation error",
				QString("%1 already exists.").arg(url.url()),
				QMessageBox::Ok
			);
			return;
		}


		auto socket = new QWebSocket();
		socket->setParent(this);

		connect(socket, &QWebSocket::disconnected, this, &client_manager::on_disconnected);
		connect(socket, &QWebSocket::connected, this, &client_manager::on_connected);
		connect(socket, &QWebSocket::errorOccurred, this, &client_manager::on_error_received);
		connect(socket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors), this, &client_manager::on_ssl_errors);

		QNetworkRequest request;

		request.setUrl(url);

		request.setRawHeader("Origin", "https://computernewb.com");

		request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");


		qDebug() << "WS: Connecting to websocket server:" << url;


		socket->open(request);

		m_clients.append(socket);

	}

	void client_manager::connect_to_servers()
	{
		for (const QString& url : m_servers) {
			add_client(QUrl(url));
		}
	}

	//Make all the clients send a specific message.
	void client_manager::broadcast(const QString& msg) const
	{
		for (QWebSocket* socket : m_clients)
			socket->sendTextMessage(msg);

		qDebug() << "WS: Broadcasted:" << msg;
	}

	void client_manager::on_connected()
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		qDebug() << "WS: WebSocket connected";
		connect(p_client, &QWebSocket::textMessageReceived, this, &client_manager::on_text_message_received);

		//TODO: Should probably request the list elsewhere.
		// Requesting list
		p_client->sendTextMessage("4.list;");
	}

	void client_manager::on_disconnected()
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());
		qDebug() << "WS: Disconnected from server: " << p_client->requestUrl();

		m_clients.removeAll(p_client);
		p_client->deleteLater();

		// Track when all clients are disconnected  
		if (m_pending_disconnections > 0) {
			m_pending_disconnections--;
			if (m_pending_disconnections == 0 && m_clients.isEmpty()) {
				emit all_clients_cleared();
			}
		}

	}

	void client_manager::on_text_message_received(const QString& message)
	{
		//NOTE: This will be a major focus for optimization later on in this projects development. (hash based lookup tables, etc.)

		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		QStringList decoded_message = guac_utils::decode(message);

		if (decoded_message[0] == "nop")
		{
			p_client->sendTextMessage("3.nop;");
			return;
		}

		if (decoded_message[0] == "adduser")
		{
			for (int i = 2; i < decoded_message.size(); i += 2) {
				emit signal_adduser_received(decoded_message[i], static_cast<user::rank>(decoded_message[i + 1].toInt()), p_client->requestUrl());
			}
			return;
		}

		if (decoded_message[0] == "remuser")
		{
			for (int i = 2; i < decoded_message.size(); ++i) {
				emit signal_remuser_received(decoded_message[i], p_client->requestUrl());
			}
			return;
		}

		if (decoded_message[0] == "flag")
		{
			for (int i = 1; i < decoded_message.size(); i += 2) {
				emit signal_flag_received(decoded_message[i], decoded_message[i + 1], p_client->requestUrl());
			}
			return;
		}

		//TODO: might be possible to skip sending the opcode through these handlers as its redundant?
		if (decoded_message[0] == "list")
		{
			// Add to list then disconnect!

			for (int i = 1; i + 2 < decoded_message.size(); i += 3) {
				emit signal_list_received(decoded_message[i], decoded_message[i + 1], decoded_message[i + 2], p_client->requestUrl());
			}

			if (!m_persistence_mode) // Dont disconnect servers after listing them in the vm list if persistence mode is enabled!
				p_client->close();

			return;
		}

		if (decoded_message[0] == "nop")
		{
			p_client->sendTextMessage("3.nop;");
			return;
		}

		qDebug() << "WS: Unimplemented/Unknown message received:" << decoded_message;
	}

	void client_manager::on_error_received(QAbstractSocket::SocketError error) const
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		QMessageBox::critical(
			nullptr,
			"Connection Error",
			QString("%1: %2").arg(p_client->requestUrl().url(), p_client->errorString()),
			QMessageBox::Ok
		);
	}

	void client_manager::on_ssl_errors(const QList<QSslError>& errors) const
	{
		Q_UNUSED(errors)

			QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		QMessageBox::critical(
			nullptr,
			"SSL Error",
			QString("%1: %2").arg(p_client->requestUrl().url(), p_client->errorString()),
			QMessageBox::Ok
		);

	}
}
