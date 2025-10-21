#pragma once
#include <qlistview.h>
#include <QMainWindow>
#include <QScopedPointer>

#include "src/cvm/models/chat_message_list.h"
#include "src/cvm/models/user_list.h"
#include "src/cvm/ws/ws_manager.h"
#include "src/settings/settings_manager.h"

namespace Ui
{
    class main_window;
}

class main_window : public QMainWindow {
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    virtual ~main_window();

    settings_manager* m_s_manager;

    cvm::ws::client_manager* m_c_manager;

    cvm::models::vm_list* m_vm_list;

    cvm::models::user_list* m_user_list;

	cvm::models::chat_message_list* m_chat_message_list;

    QHash<QString, QWidget*> m_open_vm_windows;


private Q_SLOTS:
    void on_vm_activated(const QModelIndex& index);

private:
    QScopedPointer<Ui::main_window> m_ui;
};
