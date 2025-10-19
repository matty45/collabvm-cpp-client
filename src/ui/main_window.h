#pragma once
#include <qlistview.h>
#include <QMainWindow>
#include <QScopedPointer>

namespace Ui
{
    class main_window;
}

class main_window : public QMainWindow {
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    virtual ~main_window();
    QHash<QString, QWidget*> m_open_vm_windows;

private Q_SLOTS:
    void on_vm_double_clicked(const QModelIndex& index);

private:
    QScopedPointer<Ui::main_window> ui;
};
