#pragma once
#include <QDialog>

#include "src/cvm/models/proxies/user_filter.h"

namespace Ui
{
    class vm_window;
}

class vm_window : public QWidget {
    Q_OBJECT

public:
    explicit vm_window(const cvm::vm* vm, bool persistence_mode, QWidget* parent = nullptr);
    virtual ~vm_window();

    cvm::models::proxies::user_filter* m_user_filter_proxy;
    bool m_persistence_mode;

private:
    const cvm::vm* m_vm;
    QScopedPointer<Ui::vm_window> m_ui;
};
