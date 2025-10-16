#include "ws.h"
#include "src/guac.h"

namespace cvm::ws
{
	client::client(const QUrl& url, QObject* parent) :
		QObject(parent)
	{
		connect(&m_webSocket, &QWebSocket::disconnected, this, &client::on_disconnected);
		connect(&m_webSocket, &QWebSocket::connected, this, &client::on_connected);
		connect(&m_webSocket, &QWebSocket::errorOccurred, this, &client::on_error_recieved);
		connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors), this, &client::on_ssl_errors);

		QNetworkRequest request;
		request.setUrl(url);
		request.setRawHeader("Origin", "https://computernewb.com");
		request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");

		qDebug() << "Connecting to websocket server:" << url;

		m_webSocket.open(request);
	}

	void client::on_connected()
	{
		qDebug() << "WebSocket connected";
		connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &client::on_text_message_received);
	}

	void client::on_disconnected()
	{
		qDebug() << "Disconnected from server";
		m_webSocket.close();
	}

	void client::on_text_message_received(QString message)
	{
		QStringList decoded_message = guac_utils::decode(message);

		qDebug() << "Message received:" << decoded_message;

		if (decoded_message[0] == "nop")
			m_webSocket.sendTextMessage("3.nop;");

	}

	void client::on_error_recieved(QAbstractSocket::SocketError error)
	{
		qCritical() << "Error received:" << m_webSocket.errorString();
	}

	void client::on_ssl_errors(const QList<QSslError>& errors) const
	{
		Q_UNUSED(errors)

		qCritical() << "SSL Error received:" << m_webSocket.errorString();
	}
}