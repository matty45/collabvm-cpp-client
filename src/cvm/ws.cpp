#include "ws.h"
#include "src/guac.h"

namespace cvm
{
	ws_client::ws_client(const QUrl& url, QObject* parent) :
		QObject(parent)
	{
		connect(&m_webSocket, &QWebSocket::disconnected, this, &ws_client::on_disconnected);
		connect(&m_webSocket, &QWebSocket::connected, this, &ws_client::on_connected);
		connect(&m_webSocket, &QWebSocket::errorOccurred, this, &ws_client::on_error_recieved);
		connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors), this, &ws_client::on_ssl_errors);

		QNetworkRequest request;
		request.setUrl(url);
		request.setRawHeader("Origin", "https://computernewb.com");
		request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");

		qDebug() << "WebSocket server:" << url;
		qDebug() << "WebSocket server:" << request.headers();

		m_webSocket.open(request);
	}

	void ws_client::on_connected()
	{
		qDebug() << "WebSocket connected";
		connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &ws_client::on_text_message_received);
	}

	void ws_client::on_disconnected()
	{
		qDebug() << "Disconnected from server";
		m_webSocket.close();
	}

	void ws_client::on_text_message_received(QString message)
	{
		QStringList decoded_message = guac_utils::decode(message);

		qDebug() << "Message received:" << decoded_message;

		if (decoded_message[0] == "nop")
			m_webSocket.sendTextMessage("3.nop;");

	}

	void ws_client::on_error_recieved(QAbstractSocket::SocketError error)
	{
		qCritical() << "Error received:" << m_webSocket.errorString();
	}

	void ws_client::on_ssl_errors(const QList<QSslError>& errors)
	{
		Q_UNUSED(errors)

			// WARNING: Never ignore SSL errors in production code.
			// The proper way to handle self-signed certificates is to add a custom root
			// to the CA store.

			m_webSocket.ignoreSslErrors();
	}
}