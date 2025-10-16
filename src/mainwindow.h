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
    main_window(QWidget *parent = 0);
    virtual ~main_window();
    QListView* get_vm_list_view();

private:
    QScopedPointer<Ui::main_window> ui;
};
