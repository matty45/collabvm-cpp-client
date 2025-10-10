#pragma once

#include <qobject.h>
#include <QtWebSockets/QWebSocket>

#include "cvm.h"

namespace ws
{
	void ws_test();

}

class cvm_ws_client : public QObject
{
    Q_OBJECT
public:
    explicit cvm_ws_client(const QUrl& url, QObject* parent = nullptr);

private Q_SLOTS:
    void on_connected();
    static void on_disconnected();
    static void on_text_message_received(QString message);
	void on_error_recieved(QAbstractSocket::SocketError error);
    void on_ssl_errors(const QList<QSslError>& errors);

private:
    QWebSocket m_webSocket;
};