#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QTimer>
#include "canvas.h"
#include "tcpservice.h"

class Window : public QDialog
{
    COMService &comService;
    Canvas canvas;
    QTimer timer;
    int counter{0};

    /**
     * @brief Used to redraw the canvas
     *
     */
    void refresh();

public:
    Window(COMService &comService);
};

#endif
