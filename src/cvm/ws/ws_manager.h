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

            bool m_persistence_mode; // Persistence mode lets the user choose between keeping persistent connections after vms have been listed or to disconnect from them all once they have been listed.

            QStringList m_servers; // List of servers to connect to, etc

            QWebSocket* find_client_by_url(const QUrl& url) const;
            void add_client(const QUrl& url);
            void clear_all_clients();

            void connect_to_servers();
            void broadcast(const QString& msg) const;

        signals:
            void signal_list_received(const QString& id, const QString& display_name, const QString& thumbnail, const QUrl& server);
            void all_clients_cleared();

        private Q_SLOTS:
            void on_connected();
        	void on_disconnected();
            void on_text_message_received(const QString& message);
            void on_error_received(QAbstractSocket::SocketError error) const;
            void on_ssl_errors(const QList<QSslError>& errors) const;

        private:
            QList<QWebSocket*> m_clients;
            long long m_pending_disconnections = 0;
        };
}
