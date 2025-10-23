#pragma once
#include <QDialog>

namespace Ui
{
    class vm_window;
}

class vm_window : public QWidget {
    Q_OBJECT

public:
    explicit vm_window(const cvm::vm* vm, bool persistence_mode, QWidget* parent = nullptr);
    virtual ~vm_window();

    bool m_persistence_mode;

private:
    const cvm::vm* m_vm;
    QScopedPointer<Ui::vm_window> m_ui;
};
