#ifdef UARTCOM
#include "uartservice.h"
#else
#include "tcpservice.h"
#endif

#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#ifdef UARTCOM
    UARTService service;
#else
    TCPService service;
#endif
    Window window(service);
    window.show();

    return app.exec();
}
