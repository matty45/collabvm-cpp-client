#pragma once

#include <qobject.h>
#include <QtWebSockets/QWebSocket>

namespace cvm::ws
{
        class client : public QObject
        {
            Q_OBJECT
        public:
            explicit client(const QUrl& url, QObject* parent = nullptr);

        signals:
            void signal_list_received(const QString& id, const QString& display_name, const QString& thumbnail);

        public Q_SLOTS:
            void close() const;  // Add this  

        private Q_SLOTS:
            void on_connected();
            static void on_disconnected();
            void on_text_message_received(const QString& message);
            void on_error_received(QAbstractSocket::SocketError error) const;
            void on_ssl_errors(const QList<QSslError>& errors) const;

        private:
            QSharedPointer<QWebSocket> m_webSocket;
        };
}
