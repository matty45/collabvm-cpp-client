#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ws.h"

static void handle_test_button_click()
{
    qDebug("handle_test_button_clicked");
    ws::ws_test();
}

main_window::main_window(QWidget *parent)
: QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);

    connect(ui->the_funky_test_button, &QPushButton::clicked, this, [this] { handle_test_button_click(); });

}


// this function triggers on window close/deconstruct
main_window::~main_window()
{
    
}
