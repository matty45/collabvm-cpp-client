#pragma once
#include <qlistview.h>
#include <QMainWindow>
#include <QScopedPointer>

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

    settings_manager* s_manager;

    cvm::ws::client_manager* c_manager;

    cvm::models::vm_list* vm_list;

    QHash<QString, QWidget*> m_open_vm_windows;

private Q_SLOTS:
    void on_vm_activated(const QModelIndex& index);

private:
    QScopedPointer<Ui::main_window> ui;
};
