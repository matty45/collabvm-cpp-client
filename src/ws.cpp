#include "ws.h"
#include <QWebSocketHandshakeOptions>

void ws::ws_test()
{
     cvm_ws_client client(QUrl(QStringLiteral("wss://computernewb.com/collab-vm/vm8")));
}

cvm_ws_client::cvm_ws_client(const QUrl& url,QObject* parent) :
    QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::disconnected, this, &cvm_ws_client::on_disconnected);
    connect(&m_webSocket, &QWebSocket::connected, this, &cvm_ws_client::on_connected);
    connect(&m_webSocket, &QWebSocket::errorOccurred,this, &cvm_ws_client::on_error_recieved);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),this, &cvm_ws_client::on_ssl_errors);

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Origin", "https://computernewb.com");
    request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");

    qDebug() << "WebSocket server:" << url;
    qDebug() << "WebSocket server:" << request.headers();

    m_webSocket.open(request);
}

void cvm_ws_client::on_error_recieved(QAbstractSocket::SocketError error)
{
    qCritical() << "Error received:" << m_webSocket.errorString();
}

void cvm_ws_client::on_connected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &cvm_ws_client::on_text_message_received);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void cvm_ws_client::on_disconnected()
{
    qDebug() << "Disconnected from server";
}

void cvm_ws_client::on_text_message_received(QString message)
{
    qDebug() << "Message received:" << message;
}

void cvm_ws_client::on_ssl_errors(const QList<QSslError>& errors)
{
    Q_UNUSED(errors)

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_webSocket.ignoreSslErrors();
}