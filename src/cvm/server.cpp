#include "server.h"  
#include "src/guac.h"  
#include <QDebug>  
  
namespace cvm  
{  
    server::server(const QUrl& url, QObject* parent)  
        : QObject(parent)  
        , m_websocket(nullptr)  
        , m_url(url)  
        , m_name(url.toString())  
        , m_is_connected(false)  
    {  
        // Extract server name from URL if possible  
        QString path = url.path();  
        if (!path.isEmpty()) {  
            QStringList parts = path.split('/', Qt::SkipEmptyParts);  
            if (!parts.isEmpty()) {  
                m_name = parts.last();  
            }  
        }  
    }  
      
    server::~server()  
    {  
        disconnect_from_server();  
        clear_vms();  
        clear_users();
        clear_chat_messages();
    }  
      
    // VM Management  
    vm* server::get_vm(const QString& id) const  
    {  
        for (vm* v : m_vms) {  
            if (v->m_id == id) {  
                return v;  
            }  
        }  
        return nullptr;  
    }  
      
    void server::add_vm(const QString& id, const QString& display_name, const QString& thumbnail)  
    {  
        // Check if VM already exists  
        if (get_vm(id) != nullptr) {  
            qWarning() << "WS: VM with id" << id << "already exists in server" << m_name;  
            return;  
        }  
          
        // Create new VM (it becomes owned by this server object)  
        vm* new_vm = new vm(id, display_name, thumbnail,this);
        m_vms.append(new_vm);  
          
        emit vm_added(new_vm);  
          
        qDebug() << "WS: Added VM" << id << "to server" << m_name;  
    }  
      
    void server::remove_vm(const QString& id)  
    {  
        for (int i = 0; i < m_vms.size(); ++i) {  
            if (m_vms[i]->m_id == id) {  
                vm* removed_vm = m_vms.takeAt(i);  
                emit vm_removed(removed_vm);
                delete removed_vm;  
                qDebug() << "WS: Removed VM" << id << "from server" << m_name;  
                return;  
            }  
        }  
    }  
      
    void server::clear_vms()  
    {  
        qDeleteAll(m_vms);  
        m_vms.clear();  
        emit vms_cleared();  
    }  
      
    // User Management  
    user* server::get_user(const QString& username) const  
    {  
        for (user* u : m_users) {  
            if (u->m_username == username) {  
                return u;  
            }  
        }  
        return nullptr;  
    }  
      
    void server::add_user(const QString& username, user::rank rank)  
    {  
        // Check if user already exists and if so, just update their rank 
        if (get_user(username) != nullptr) {
            update_user_rank(username, rank);
            return;  
        }  
          
        // Create new user  
        user* new_user = new user(username, rank);  
        m_users.append(new_user);
        m_user_count++;
          
        emit user_joined(new_user);  
          
        qDebug() << "WS: User" << username << "joined server" << m_name;  
    }  
      
    void server::remove_user(const QString& username)  
    {  
        for (int i = 0; i < m_users.size(); ++i) {  
            if (m_users[i]->m_username == username) {  
                user* removed_user = m_users.takeAt(i);  
                emit user_left(username);  
                delete removed_user;
                m_user_count--;
                qDebug() << "WS: User" << username << "left server" << m_name;  
                return;  
            }  
        }  
    }

    void server::update_username(const QString& old_username, const QString& new_username)
    {
        user* u = get_user(old_username);
        if (u) {
            u->m_username = new_username;
            qDebug() << "WS: Updating username for user " << old_username << "to:" << new_username << "in server" << m_name;
            emit user_updated(u);
        }
    }

    void server::update_user_rank(const QString& username, user::rank rank)
    {
        user* u = get_user(username);
        if (u) {
            u->m_rank = rank;
            qDebug() << "WS: Updating rank for user " << username << "to:" << rank << "in server" << m_name;
            emit user_updated(u);
        }
    }
      
    void server::update_user_country(const QString& username, const QString& country_code)  
    {  
        user* u = get_user(username);  
        if (u) {  
            u->m_country_code = country_code;
            qDebug() << "WS: Received country code" << country_code << "for user:" << username << "in server" << m_name;
            emit user_updated(u);  
        }  
    }  
      
    void server::clear_users()  
    {  
        qDeleteAll(m_users);  
        m_users.clear();  
        emit users_cleared();  
    }  

    // Chat message management
    void server::add_chat_message(const QString& sender, const QString& message)
    {
        // Get user from sender param - if the param is null its a system or motd message.

        user* sender_user = get_user(sender);

        qDebug() << "WS: Chat message from" << sender << ":" << message << "in server:" << m_name;

        chat_message* chat_msg = new chat_message{ sender_user,message,this };
        m_chat_messages.append(chat_msg);

        emit chat_message_created(chat_msg);
    }

    void server::clear_chat_messages()
    {
        qDeleteAll(m_chat_messages);
        m_chat_messages.clear();
        emit chat_messages_cleared();
    }

    // Connection Management  
    void server::connect_to_server()  
    {  
        if (m_websocket) {  
            qWarning() << "WS: Already connected or connecting to" << m_url;  
            return;  
        }  
          
        m_websocket = new QWebSocket();  
        m_websocket->setParent(this);  
          
        // Connect signals  
        connect(m_websocket, &QWebSocket::connected, this, &server::on_websocket_connected);  
        connect(m_websocket, &QWebSocket::disconnected, this, &server::on_websocket_disconnected);  
        connect(m_websocket, &QWebSocket::errorOccurred, this, &server::on_websocket_error);  
        connect(m_websocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),   
                this, &server::on_ssl_errors);  
        connect(m_websocket, &QWebSocket::textMessageReceived, this, &server::on_text_message_received);  
          
        // Prepare request  
        QNetworkRequest request;  
        request.setUrl(m_url);  
        request.setRawHeader("Origin", "https://computernewb.com");  
        request.setRawHeader("Sec-WebSocket-Protocol", "guacamole");  
          
        qDebug() << "WS: Connecting to server:" << m_url;  
        m_websocket->open(request);  
    }  

    void server::reconnect()
    {
        qDebug() << "WS: Reconnecting to server:" << m_name;
        disconnect_from_server();
        connect_to_server();
    }

    void server::disconnect_from_server()  
    {  
        if (m_websocket) {  
            m_websocket->close();  
            m_websocket->deleteLater();  
            m_websocket = nullptr;  
        }  
        m_is_connected = false;  
    }  
      
    void server::send_message(const QString& message) const
    {  
        if (m_websocket && m_is_connected) {  
            m_websocket->sendTextMessage(message);  
        }  
    }  
      
    // WebSocket Slots  
    void server::on_websocket_connected()  
    {  
        m_is_connected = true;  
        qDebug() << "WS: Connected to server:" << m_name;  
        emit connected();  
          
        // Request VM list  
        send_message("4.list;");  
    }  
      
    void server::on_websocket_disconnected()  
    {  
        m_is_connected = false;  
        qDebug() << "Disconnected from server:" << m_name;  
          
        // Clear all data when disconnected  
        clear_vms();  
        clear_users();  
          
        emit disconnected();  
    }  
      
    void server::on_websocket_error(QAbstractSocket::SocketError error)  
    {  
        QString error_string = m_websocket ? m_websocket->errorString() : "Unknown error";  
        qCritical() << "WS: Server" << m_name << "error:" << error_string;  
        emit error_occurred(error_string);  
    }  
      
    void server::on_ssl_errors(const QList<QSslError>& errors) const
    {  
        qWarning() << "WS: SSL errors for server" << m_name;  
    }  
      
    void server::on_text_message_received(const QString& message)  
    {  
        QStringList decoded = guac_utils::decode(message);  
          
        if (decoded.isEmpty()) {  
            return;  
        }  
          
        const QString& opcode = decoded[0];  
          
        // Handle nop (heartbeat)  
        if (opcode == "nop") {  
            send_message("3.nop;");  
            return;  
        }  
          
        // Handle list (VM list)  
        if (opcode == "list") {  
            handle_list_message(decoded);  
            return;  
        }  
          
        // Handle adduser (user joined)  
        if (opcode == "adduser") {  
            handle_adduser_message(decoded);  
            return;  
        }  
          
        // Handle remuser (user left)  
        if (opcode == "remuser") {  
            handle_remuser_message(decoded);  
            return;  
        }

        // Handle rename (user renamed themselves)  
        if (opcode == "rename") {
            handle_remuser_message(decoded);
            return;
        }

        // Handle flag (add user country)
        if (opcode == "flag")
        {
            handle_flag_message(decoded);
            return;
        }

        // Handle chat (add chat message)
        if (opcode == "chat")
        {
            handle_chat_message(decoded);
            return;
        }
          
        // Log unhandled messages  
        qDebug() << "WS: Unhandled message from" << m_name << ":" << decoded;  
    }  
      
    // Message Handlers
    void server::handle_chat_message(const QStringList& decoded)
    {
    	add_chat_message(decoded[1], decoded[2]);
    }

    void server::handle_list_message(const QStringList& decoded)  
    {  
        // Clear existing VMs before adding new ones  
        clear_vms();  
          
        // Format: list, id1, name1, thumb1, id2, name2, thumb2, ...  
        for (int i = 1; i + 2 < decoded.size(); i += 3) {  
            add_vm(decoded[i], decoded[i + 1], decoded[i + 2]);  
        }

        emit vm_list_received();
    }  

    void server::handle_flag_message(const QStringList& decoded)
    {
        for (int i = 1; i < decoded.size(); i += 2) {
        	update_user_country(decoded[i], decoded[i + 1]);
        }
    }

    void server::handle_adduser_message(const QStringList& decoded)  
    {
        for (int i = 2; i < decoded.size(); i += 2) {
            add_user(decoded[i], static_cast<user::rank>(decoded[i + 1].toInt()));
        }
    }  
      
    void server::handle_remuser_message(const QStringList& decoded)  
    {  
        for (int i = 2; i < decoded.size(); ++i) {
            remove_user(decoded[i]);
        }
    }

    void server::handle_rename_message(const QStringList& decoded)
    {
        if (decoded[1] == "1") //another user in the list is renamed
        {
            update_username(decoded[2], decoded[3]);
        }
        else // Client rename result
            Q_UNIMPLEMENTED();
    }
}