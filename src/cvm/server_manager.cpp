#include "server_manager.h"  

namespace cvm
{
    server_manager::server_manager(QObject* parent)
	    : QObject(parent), m_persistence_mode(false)
    {
    }

    server_manager::~server_manager()
    {
        disconnect_all_servers();
        qDeleteAll(m_servers);
    }

    void server_manager::on_server_vm_list_received() const
    {
        if (!m_persistence_mode) {
            server* rip = qobject_cast<server*>(sender());
            rip->clear_chat_messages();
            rip->clear_users();
            rip->disconnect_from_server();
        }
    }

    server* server_manager::add_server(const QUrl& url)
    {
        // Check if server already exists  
        if (get_server(url) != nullptr) {
            qWarning() << "Server" << url << "already exists";
            return get_server(url);
        }

        server* new_server = new server(url, this);
        connect(new_server, &server::vm_list_received, this, &server_manager::on_server_vm_list_received);

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

    void server_manager::reconnect_all()
    {
        qDebug() << "Reconnecting to all servers...";
        emit all_servers_reconnecting();

        for (server* srv : m_servers) {
            srv->reconnect();
        }
    }

    void server_manager::connect_all_servers()
    {
        for (server* s : m_servers) {
            s->connect_to_server();
        }
    }

    void server_manager::broadcast_message_to_all_servers(const QString& msg)
    {
        for (server* s : m_servers) {
            s->send_message(msg);
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