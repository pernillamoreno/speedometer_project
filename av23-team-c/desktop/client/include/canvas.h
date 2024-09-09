#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QSoundEffect>

class Canvas : public QWidget
{

public:
    explicit Canvas(QWidget *parent = nullptr);

    void setLeftArrow(bool value);
    void setRightArrow(bool value);
    void setWarning(bool value);
    void setTemperature(int temp);
    void setSpeed(int newSpeed);
    void setBatteryLevel(int level);
    void setConnectionStatus(bool connected);
    void updateIndicators();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void playSound();
    void stopSound();
    void updateSound();
    void drawBackground(QPainter &painter);
    void drawArc(QPainter &painter, const QPointF &center);
    void drawCenterDots(QPainter &painter, const QPointF &center);
    void drawNeedle(QPainter &painter, const QPointF &center);
    void drawIcons(QPainter &painter, const QPointF &center);
    void drawThermostat(QPainter &painter, const QPointF &center);
    void drawConnectionStatus(QPainter &painter, const QPointF &center);
    void drawBattery(QPainter &painter, const QPointF &center);
    void drawSpeedometer(QPainter &painter, const QPointF &center);
    void drawSpeedometerArc(QPainter &painter, QPoint center, int radius);
    void drawSpeedometerLines(QPainter &painter, QPoint center, int radius);

    bool isConnected;
    bool leftArrowOn;
    bool rightArrowOn;
    bool warningOn;
    bool warningVisible;
    bool leftArrowVisible;
    bool rightArrowVisible;
    int temperature;
    int batteryLevel;
    int speed;
    QSoundEffect soundEffect;
};

#endif // CANVAS_H
