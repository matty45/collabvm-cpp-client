#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ws.h"

main_window::main_window(QWidget *parent)
: QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);

    //connect(ui->the_funky_test_button, &QPushButton::clicked, this, [this] { handle_test_button_click(); });

    //connect(ui->the_funky_test_button, &QPushButton::clicked, this, [this] {ws::client = new cvm_ws_client(QUrl(QStringLiteral("wss://computernewb.com/collab-vm/vm8")), this); });

}

// this function triggers on window close/deconstruct
main_window::~main_window()
{
    
}
