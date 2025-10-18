#include "settings_dialog.h"

#include "ui_settings_dialog.h"

settings_dialog::settings_dialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::settings_dialog)
{

    ui->setupUi(this);
   
}

settings_dialog::~settings_dialog()
{
}