#include "ws_manager.h"
#include "src/guac.h"

namespace cvm::ws
{
	client_manager::client_manager( QObject* parent) :
		QObject(parent)
	{
	}

	client_manager::~client_manager()
	{

		qDebug() << "Shutting down client manager!";

		for (QWebSocket* socket : m_clients) {
			socket->close();
		}

		qDeleteAll(m_clients.begin(), m_clients.end());
	}

	// Researched how KDE manages multiple websockets. https://github.com/KDE/tokodon/blob/master/src/account/account.h 
	void client_manager::add_client(QUrl url)
	{
		for (QWebSocket* socket : m_clients) {
			if (socket && socket->requestUrl() == url) {
				return;
			}
		}

		//TODO: Move connection logic into its own function.
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

		qDebug() << "Connecting to websocket server:" << url;

		socket->open(request);

		m_clients.append(socket);
	}

	void client_manager::on_connected()
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		qDebug() << "WebSocket connected";
		connect(p_client, &QWebSocket::textMessageReceived, this, &client_manager::on_text_message_received);

		// Requesting list
		p_client->sendTextMessage("4.list;");
	}

	void client_manager::on_disconnected()
	{
		qDebug() << "Disconnected from server";

		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		if (p_client)
		{
			m_clients.removeAll(p_client);
			p_client->deleteLater();
		}
	}

	void client_manager::on_text_message_received(const QString& message)
	{
		//NOTE: This will be a major focus for optimization later on in this projects development. (hash based lookup tables, etc.)

		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		QStringList decoded_message = guac_utils::decode(message);

		qDebug() << "Message received:" << decoded_message;

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
	}

	void client_manager::on_error_received(QAbstractSocket::SocketError error) const
	{
		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		qCritical() << "Error received:" << p_client->errorString();
	}

	void client_manager::on_ssl_errors(const QList<QSslError>& errors) const
	{
		Q_UNUSED(errors)

		QWebSocket* p_client = qobject_cast<QWebSocket*>(sender());

		qCritical() << "SSL Error received:" << p_client->errorString();
	}
}
