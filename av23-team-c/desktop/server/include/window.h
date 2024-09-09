#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include "setting.h"
#include "tcpservice.h"

class Window : public QDialog
{
public:
    explicit Window(COMService &service);
    ~Window();

private slots:
    void onleftCheckBoxChanged(int state);
    void onrightCheckBoxChanged(int state);
    void onWarningCheckBoxChanged(int state);

private:
    void setupUi();
    void connectSignals();

    QSlider speedSlider{Qt::Horizontal};
    QSlider temperatureSlider{Qt::Horizontal};
    QSlider batterySlider{Qt::Horizontal};
    QCheckBox leftCheckBox{"Left"};
    QCheckBox rightCheckBox{"Right"};
    QCheckBox warningCheckBox{"Warning"};
    QLabel speedLabel{"Speed:"};
    QLabel temperatureLabel{"Temperature:"};
    QLabel batteryLabel{"Battery Level:"};
    QLabel lightSignalsLabel{"Light Signals:"};
    QLabel speedValueLabel{"0 Kph"};
    QLabel temperatureValueLabel{"0 °C"};
    QLabel batteryValueLabel{"0%"};

    QVBoxLayout mainLayout{this}; // member variable
    QGridLayout gridLayout;
    QHBoxLayout checkboxLayout;

    COMService &comService;
};

#endif // WINDOW_H

