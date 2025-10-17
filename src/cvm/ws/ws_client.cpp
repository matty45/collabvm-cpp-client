#include "ws_client.h"
#include "src/guac.h"

namespace cvm::ws
{
	client::client(const QUrl& url, QObject* parent) :
		QObject(parent)
	{

		m_webSocket = QSharedPointer<QWebSocket>(new QWebSocket());

		connect(m_webSocket.data(), &QWebSocket::disconnected, this, &client::on_disconnected);
		connect(m_webSocket.data(), &QWebSocket::connected, this, &client::on_connected);
		connect(m_webSocket.data(), &QWebSocket::errorOccurred, this, &client::on_error_received);
		connect(m_webSocket.data(), QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors), this, &client::on_ssl_errors);

		QNetworkRequest request;
		request.setUrl(url);
		request.setRawHeader("Origin", "https://computernewb.com");
		request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");

		qDebug() << "Connecting to websocket server:" << url;

		m_webSocket->open(request);
	}

	void client::close()
	{
		qDebug() << "Closing WebSocket client";
		m_webSocket->close();
	}

	void client::on_connected()
	{
		qDebug() << "WebSocket connected";
		connect(m_webSocket.data(), &QWebSocket::textMessageReceived, this, &client::on_text_message_received);

		// Requesting list
		m_webSocket->sendTextMessage("4.list;");
	}

	void client::on_disconnected()
	{
		qDebug() << "Disconnected from server";
	}

	void client::on_text_message_received(const QString& message)
	{
		//NOTE: This will be a major focus for optimization later on in this projects development. (hash based lookup tables, etc.)

		QStringList decoded_message = guac_utils::decode(message);

		qDebug() << "Message received:" << decoded_message;

		if (decoded_message[0] == "nop")
			m_webSocket->sendTextMessage("3.nop;");

		//TODO: might be possible to skip sending the opcode through these handlers as its redundant?
		if (decoded_message[0] == "list")
			for (int i = 1; i + 2 < decoded_message.size(); i += 3) {
				emit signal_list_received(decoded_message[i], decoded_message[i + 1], decoded_message[i + 2]);
			}

	}

	void client::on_error_received(QAbstractSocket::SocketError error) const
	{
		qCritical() << "Error received:" << m_webSocket->errorString();
	}

	void client::on_ssl_errors(const QList<QSslError>& errors) const
	{
		Q_UNUSED(errors)

		qCritical() << "SSL Error received:" << m_webSocket->errorString();
	}
}