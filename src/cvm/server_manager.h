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

        // Server management  
        server* add_server(const QUrl& url);
        void remove_server(const QUrl& url);
        server* get_server(const QUrl& url) const;
        QList<server*> servers() const { return m_servers; }

        // Global operations  
        void connect_all_servers();
        void disconnect_all_servers();

        // Global queries across all servers  
        QList<vm*> all_vms() const;
        QList<user*> all_users() const;
        int total_vm_count() const;
        int total_user_count() const;

    signals:
        void server_added(server* server);
        void server_removed(const QUrl& url);

    private:
        QList<server*> m_servers;
    };
}