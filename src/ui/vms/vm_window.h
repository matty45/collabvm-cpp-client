#pragma once
#include <QScopedPointer>
#include <QDialog>

#include "src/ui/main_window.h"

namespace Ui
{
    class vm_window;
}

class vm_window : public QWidget {
    Q_OBJECT

public:
    explicit vm_window(const cvm::vm& vm,QWidget* parent = nullptr);
    virtual ~vm_window();

private:
    QScopedPointer<Ui::vm_window> m_ui;
};
