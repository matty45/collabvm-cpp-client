#pragma once  
#include <QObject>  
#include <QList>  
#include "server.h"  

namespace cvm
{
    class server_manager : public QObject
    {
        Q_OBJECT

    public:
        explicit server_manager(QObject* parent = nullptr);
        ~server_manager();

        bool m_persistence_mode; // Persistence mode lets the user choose between keeping persistent connections after vms have been listed or to disconnect from them all once they have been listed.

        // Server management
        server* add_server(const QUrl& url);
        void remove_server(const QUrl& url);
        server* get_server(const QUrl& url) const;
        QList<server*> servers() const { return m_servers; }

        // Global operations  
        void connect_all_servers();
        void broadcast_message_to_all_servers(const QString& msg);
        void disconnect_all_servers();
        void reconnect_all();

        // Global queries across all servers  
        QList<vm*> all_vms() const;
        QList<user*> all_users() const;
        int total_vm_count() const;
        int total_user_count() const;

    signals:
        void server_added(server* server);
        void server_removed(const QUrl& url);

    private slots:
        void on_server_vm_list_received() const;

    private:
        QList<server*> m_servers;
    };
}