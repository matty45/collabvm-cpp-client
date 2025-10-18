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
    explicit main_window(QWidget *parent = 0);
    virtual ~main_window();

private Q_SLOTS:
    void on_action_open_settings_triggered();

private:
    QScopedPointer<Ui::main_window> ui;
};
