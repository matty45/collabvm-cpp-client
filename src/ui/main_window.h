#pragma once
#include <qlistview.h>
#include <QMainWindow>
#include <QScopedPointer>

#include "src/cvm/server_manager.h"
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

    settings_manager* m_settings_manager;

    cvm::server_manager* m_server_manager;

    cvm::models::vm_list* m_vm_list_model;



private Q_SLOTS:
    void on_vm_activated(const QModelIndex& index);

private:
    QScopedPointer<Ui::main_window> m_ui;
};
