#include "server_manager.h"  

namespace cvm
{
    server_manager::server_manager(QObject* parent)
        : QObject(parent)
    {
    }

    server_manager::~server_manager()
    {
        disconnect_all_servers();
        qDeleteAll(m_servers);
    }

    server* server_manager::add_server(const QUrl& url)
    {
        // Check if server already exists  
        if (get_server(url) != nullptr) {
            qWarning() << "Server" << url << "already exists";
            return get_server(url);
        }

        server* new_server = new server(url, this);
        m_servers.append(new_server);

        emit server_added(new_server);

        return new_server;
    }

    void server_manager::remove_server(const QUrl& url)
    {
        for (int i = 0; i < m_servers.size(); ++i) {
            if (m_servers[i]->url() == url) {
                server* removed = m_servers.takeAt(i);
                emit server_removed(url);
                delete removed;
                return;
            }
        }
    }

    server* server_manager::get_server(const QUrl& url) const
    {
        for (server* s : m_servers) {
            if (s->url() == url) {
                return s;
            }
        }
        return nullptr;
    }

    void server_manager::connect_all_servers()
    {
        for (server* s : m_servers) {
            s->connect_to_server();
        }
    }

    void server_manager::disconnect_all_servers()
    {
        for (server* s : m_servers) {
            s->disconnect_from_server();
        }
    }

    QList<vm*> server_manager::all_vms() const
    {
        QList<vm*> result;
        for (server* s : m_servers) {
            result.append(s->vms());
        }
        return result;
    }

    QList<user*> server_manager::all_users() const
    {
        QList<user*> result;
        for (server* s : m_servers) {
            result.append(s->users());
        }
        return result;
    }

    int server_manager::total_vm_count() const
    {
        int count = 0;
        for (server* s : m_servers) {
            count += s->vms().size();
        }
        return count;
    }

    int server_manager::total_user_count() const
    {
        int count = 0;
        for (server* s : m_servers) {
            count += s->users().size();
        }
        return count;
    }
}