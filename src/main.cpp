#include <QApplication>

#include "mainwindow.h"
#include "ws.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_window;
    main_window.show();

    //TODO: Somehow move this logic elsewhere (it only seems to work in main.cpp for some screwed up reason.)
    cvm_ws_client client(QUrl(QStringLiteral("wss://computernewb.com/collab-vm/vm8")));

    return app.exec();
}
