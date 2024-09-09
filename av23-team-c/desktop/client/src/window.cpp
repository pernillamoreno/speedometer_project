#include "window.#include "window.h"

Window::Window(COMService &service)
    : comService(service)
{
    canvas.setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    canvas.setFixedSize(800, 550);
    canvas.show();

    timer.setInterval(Setting::INTERVAL);
    connect(&timer, &QTimer::timeout, this, &Window::refresh);
    timer.start();
}

void Window::refresh()
{
    if (comService.getStatus())
    {
        canvas.setSpeed(comService.getSpeed());
        canvas.setTemperature(comService.getTemperature());
        canvas.setBatteryLevel(comService.getBatteryLevel());
        canvas.setRightArrow(comService.getRight());
        canvas.setLeftArrow(comService.getLeft());
        canvas.setConnectionStatus(true);
    }
    else
    {
        canvas.setSpeed(0);
        canvas.setTemperature(0);
        canvas.setBatteryLevel(0);
        canvas.setRightArrow(false);
        canvas.setLeftArrow(false);
        canvas.setConnectionStatus(false);
    }

    counter++;
    if (counter % 8 == 0)
    {
        canvas.updateIndicators();
        counter = 0;
    }

    canvas.update();
}
h"
