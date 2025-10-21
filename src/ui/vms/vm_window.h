#pragma once
#include <QScopedPointer>
#include <QDialog>

#include "src/cvm/models/proxies/user_filter.h"

namespace Ui
{
    class vm_window;
}

class vm_window : public QWidget {
    Q_OBJECT

public:
    explicit vm_window(const cvm::vm& vm, QAbstractListModel* user_list_model, QWidget* parent = nullptr);
    virtual ~vm_window();

    cvm::models::proxies::user_filter* m_user_filter_proxy;

private:
    QScopedPointer<Ui::vm_window> m_ui;
};
