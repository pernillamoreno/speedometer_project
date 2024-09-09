#include "wi#include "window.h"
#include <QDebug>

Window::Window(COMService &comService) : comService(comService)
{
    setWindowTitle("Server");

    setFixedSize(800, 150);

    // Ensure the window is non-resizable, stays on top, and has minimize and close buttons
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);

    setupUi();
    connectSignals();
}

Window::~Window() {}

void Window::setupUi()
{
    speedSlider.setFixedSize(600, 30);
    temperatureSlider.setFixedSize(600, 30);
    batterySlider.setFixedSize(600, 30);

    speedSlider.setRange(comService.signal["speed"].min, comService.signal["speed"].max);
    temperatureSlider.setRange(comService.signal["temperature"].min, comService.signal["temperature"].max);
    batterySlider.setRange(comService.signal["battery_level"].min, comService.signal["battery_level"].max);

    // Connect sliders to value labels
    connect(&speedSlider, &QSlider::valueChanged, [this](int value)
            {
        comService.setSpeed(value);
        speedValueLabel.setText(QString::number(value) + " Kph"); });

    connect(&temperatureSlider, &QSlider::valueChanged, [this](int value)
            {
        comService.setTemperature(value);
        temperatureValueLabel.setText(QString::number(value) + " °C"); });

    connect(&batterySlider, &QSlider::valueChanged, [this](int value)
            {
        comService.setBatteryLevel(value);
        batteryValueLabel.setText(QString::number(value) + "%"); });

    // Add widgets to the grid layout
    gridLayout.addWidget(&speedLabel, 0, 0, Qt::AlignRight);
    gridLayout.addWidget(&speedSlider, 0, 1);
    gridLayout.addWidget(&speedValueLabel, 0, 2, Qt::AlignLeft);

    gridLayout.addWidget(&temperatureLabel, 1, 0, Qt::AlignRight);
    gridLayout.addWidget(&temperatureSlider, 1, 1);
    gridLayout.addWidget(&temperatureValueLabel, 1, 2, Qt::AlignLeft);

    gridLayout.addWidget(&batteryLabel, 2, 0, Qt::AlignRight);
    gridLayout.addWidget(&batterySlider, 2, 1);
    gridLayout.addWidget(&batteryValueLabel, 2, 2, Qt::AlignLeft);

    // Add the light signals label and checkboxes
    gridLayout.addWidget(&lightSignalsLabel, 3, 0, Qt::AlignRight);
    checkboxLayout.addWidget(&leftCheckBox);
    checkboxLayout.addWidget(&rightCheckBox);
    checkboxLayout.addWidget(&warningCheckBox);
    gridLayout.addLayout(&checkboxLayout, 3, 1, 1, 2);

    mainLayout.addLayout(&gridLayout);
    setLayout(&mainLayout);
}

void Window::connectSignals()
{
    connect(&leftCheckBox, &QCheckBox::stateChanged, this, &Window::onleftCheckBoxChanged);
    connect(&rightCheckBox, &QCheckBox::stateChanged, this, &Window::onrightCheckBoxChanged);
    connect(&warningCheckBox, &QCheckBox::stateChanged, this, &Window::onWarningCheckBoxChanged);
}

void Window::onleftCheckBoxChanged(int state)
{
    rightCheckBox.setEnabled(state != Qt::Checked);

    if (!warningCheckBox.isChecked())
    {
        comService.setLeft(state == Qt::Checked);
    }
}

void Window::onrightCheckBoxChanged(int state)
{
    leftCheckBox.setEnabled(state != Qt::Checked);

    if (!warningCheckBox.isChecked())
    {
        comService.setRight(state == Qt::Checked);
    }
}

void Window::onWarningCheckBoxChanged(int state)
{
    if (state == Qt::Checked)
    {
        comService.setRight(true);
        comService.setLeft(true);
    }
    else
    {
        comService.setRight(rightCheckBox.isChecked());
        comService.setLeft(leftCheckBox.isChecked());
    }
}

