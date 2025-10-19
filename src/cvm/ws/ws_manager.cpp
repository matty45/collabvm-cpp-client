#include "ws_manager.h"
#include "src/guac.h"
#include "QMessageBox"

namespace cvm::ws
{
	client_manager::client_manager(QObject* parent) :
		QObject(parent)
	{
	}

	client_manager::~client_manager()
	{

		qDebug() << "Shutting down client manager!";

		for (QWebSocket* socket : m_clients)
			socket->close();

		qDeleteAll(m_clients.begin(), m_clients.end());

	}

	QWebSocket* client_manager::find_client_by_url(const QUrl& url) const
	{
		if (m_clients.contains(url))
			return m_clients.value(url);

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

		m_clients.insert(url, socket);
	}


	void client_manager::connect_client(const QUrl& url) const
	{
		QWebSocket* socket = find_client_by_url(url);

		if (!socket)
		{
			QMessageBox::critical(
				nullptr,
				"Websocket client connection error",
				QString("%1 does not exist.").arg(url.url()),
				QMessageBox::Ok
			);
			return;
		}

		QNetworkRequest request;

		request.setUrl(url);

		request.setRawHeader("Origin", "https://computernewb.com");

		request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");


		qDebug() << "WS: Connecting to websocket server:" << url;


		socket->open(request);
	}

	void client_manager::on_connected()
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		qDebug() << "WS: WebSocket connected";
		connect(p_client, &QWebSocket::textMessageReceived, this, &client_manager::on_text_message_received);

		// Requesting list
		p_client->sendTextMessage("4.list;");
	}

	void client_manager::on_disconnected() const
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());
		qDebug() << "WS: Disconnected from server: " << p_client->requestUrl();
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


		//TODO: might be possible to skip sending the opcode through these handlers as its redundant?
		if (decoded_message[0] == "list")
		{
			for (int i = 1; i + 2 < decoded_message.size(); i += 3) {
				emit signal_list_received(decoded_message[i], decoded_message[i + 1], decoded_message[i + 2]);
			}
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
