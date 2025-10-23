#pragma once
#include <QDialog>

namespace Ui
{
    class settings_dialog;
}

class settings_dialog : public QDialog {
    Q_OBJECT

public:
    explicit settings_dialog(QWidget *parent = nullptr);
    virtual ~settings_dialog();

private:
    QScopedPointer<Ui::settings_dialog> m_ui;
};
