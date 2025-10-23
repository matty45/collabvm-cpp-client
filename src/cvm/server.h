#pragma once  
#include <QList>  
#include <QObject>  
#include <QUrl>  
#include <QtWebSockets/QWebSocket>

#include "models/chat_message_list.h"
#include "models/user_list.h"

namespace cvm
{

	class server : public QObject
    {
        Q_OBJECT

    public:
        explicit server(const QUrl& url, QObject* parent = nullptr);
        ~server();

        // Getters  
        QUrl url() const { return m_url; }
        QString name() const { return m_name; }
        QList<vm*> vms() const { return m_vms; }
        models::user_list* user_model() const { return m_user_list_model; }
        models::chat_message_list* chat_model() const { return m_chat_messages_model; }
        int user_count() const { return m_user_count; }
        bool is_connected() const { return m_is_connected; }

        // VM management  
        vm* get_vm(const QString& id) const;
        void add_vm(const QString& id, const QString& display_name, const QString& thumbnail);
        void remove_vm(const QString& id);
        void clear_vms();

        // User management  
        void update_user_rank(const QString& username, user::rank rank);
        void add_user(const QString& username, user::rank rank);
        void remove_user(const QString& username);
        void update_username(const QString& old_username, const QString& new_username);
        void update_user_country(const QString& username, const QString& country_code);
        void clear_users();

        // Chat message management
        void add_chat_message(const QString& sender, const QString& message);
        void clear_chat_messages();

        // Connection management  
        void connect_to_server(bool skip_list_request = false);
        void reconnect();
        void disconnect_from_server();
        void send_message(const QString& message) const;

    signals:
        // Server signals  
        void connected();
        void disconnected();
        void error_occurred(const QString& error);

        // VM signals
        void vm_list_received(); //triggers on 4.list; response
        void vm_added(vm* vm);
        void vm_removed(vm* vm);
        void vms_cleared();

        // User signals  
        void user_joined(user* user);
        void user_left(user* user);
        void user_updated(user* user);
        void users_cleared();

        // Chat message signals
        void chat_message_created(chat_message* chat_msg);
        void chat_messages_cleared();

    private slots:
        void on_websocket_connected();
        void on_websocket_disconnected();
        void on_websocket_error(QAbstractSocket::SocketError error);
        void on_ssl_errors(const QList<QSslError>& errors) const;
        void on_text_message_received(const QString& message);

    private:
        // WebSocket connection  
        QWebSocket* m_websocket;
        QUrl m_url;
        QString m_name;
        bool m_is_connected;

        // Data storage
        int m_user_count = 0;
        bool m_skip_request_list = false; // Option to skip vm listing on connect.
        QList<vm*> m_vms;
        models::user_list* m_user_list_model;
        models::chat_message_list* m_chat_messages_model;


        // Helper methods
        void handle_chat_message(const QStringList& decoded);
        void handle_list_message(const QStringList& decoded);
        void handle_flag_message(const QStringList& decoded);
        void handle_adduser_message(const QStringList& decoded);
        void handle_remuser_message(const QStringList& decoded);
        void handle_rename_message(const QStringList& decoded);
    };
}
