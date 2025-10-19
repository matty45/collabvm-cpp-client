#pragma once

#include <qobject.h>
#include <QtWebSockets/QWebSocket>

namespace cvm::ws
{
        class client_manager : public QObject
        {
            Q_OBJECT
        public:
            explicit client_manager(QObject* parent = nullptr);
            ~client_manager();
            QWebSocket* find_client_by_url(const QUrl& url) const;
            void add_client(const QUrl& url);
            void connect_client(const QUrl& url);

        signals:
            void signal_list_received(const QString& id, const QString& display_name, const QString& thumbnail);
            void closed();

        private Q_SLOTS:
            void on_connected();
        	void on_disconnected();
            void on_text_message_received(const QString& message);
            void on_error_received(QAbstractSocket::SocketError error) const;
            void on_ssl_errors(const QList<QSslError>& errors) const;

        private:
            QHash<QUrl,QWebSocket*> m_clients;
        };
}
