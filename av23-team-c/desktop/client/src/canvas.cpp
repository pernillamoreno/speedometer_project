#include#include "canvas.h"
#include <QPainter>
#include <QRectF>
#include <QPen>
#include <QLinearGradient>
#include <cmath>
#include <QDir>
#include <QtMultimedia/QSoundEffect>

namespace
{
    const int ARC_WIDTH = 600;
    const int ARC_HEIGHT = 600;
    const int START_ANGLE = 330 * 16;
    const int SPAN_ANGLE = 240 * 16;
    const QPointF CENTER_OFFSET(-30, 70);
}

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
      isConnected(false),
      leftArrowOn(false),
      rightArrowOn(false),
      warningOn(false),
      warningVisible(false),
      leftArrowVisible(false),
      rightArrowVisible(false),
      temperature(0),
      batteryLevel(0),
      speed(0)
{
    setWindowTitle("Client");

    QString soundPath = QDir::currentPath() + "/../desktop/client/res/turn-signals.wav";
    soundEffect.setSource(QUrl::fromLocalFile(soundPath));
    soundEffect.setVolume(0.5f);
}

void Canvas::setLeftArrow(bool value) { leftArrowOn = value; }
void Canvas::setRightArrow(bool value) { rightArrowOn = value; }
void Canvas::setWarning(bool value)
{
    warningOn = value;
    warningVisible = value;
}
void Canvas::setTemperature(int temp) { temperature = temp; }
void Canvas::setSpeed(int newSpeed) { speed = newSpeed; }
void Canvas::setBatteryLevel(int level) { batteryLevel = level; }
void Canvas::setConnectionStatus(bool connected) { isConnected = connected; }

void Canvas::playSound()
{
    if (warningOn || leftArrowVisible || rightArrowVisible)
    {
        if (soundEffect.isPlaying())
            soundEffect.stop();
        soundEffect.play();
    }
}

void Canvas::stopSound()
{
    if (soundEffect.isPlaying())
        soundEffect.stop();
}

void Canvas::updateSound()
{
    if (leftArrowVisible || rightArrowVisible || warningOn)
        playSound();
    else
        stopSound();
}

void Canvas::updateIndicators()
{
    static bool blinkState{false};
    blinkState = !blinkState;
    leftArrowVisible = blinkState && leftArrowOn;
    rightArrowVisible = blinkState && rightArrowOn;
    updateSound();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF center(width() / 2, height() / 2);
    center += CENTER_OFFSET;

    drawBackground(painter);
    drawArc(painter, center);
    drawCenterDots(painter, center);
    drawNeedle(painter, center);
    drawIcons(painter, center);
    drawThermostat(painter, center);
    drawConnectionStatus(painter, center);
    drawBattery(painter, center);
    drawSpeedometer(painter, center);
}

void Canvas::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(30, 0, 30));

    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0.0, QColor(0, 0, 230));
    gradient.setColorAt(1.0, QColor(0, 0, 0));
    painter.fillRect(rect(), gradient);
}

void Canvas::drawArc(QPainter &painter, const QPointF &center)
{
    QRectF rectangle(center.x() - ARC_WIDTH / 2, center.y() - ARC_HEIGHT / 2, ARC_WIDTH, ARC_HEIGHT);

    QPen pen(Qt::white);
    pen.setWidth(8);
    painter.setPen(pen);
    painter.drawArc(rectangle, START_ANGLE, SPAN_ANGLE);
}

void Canvas::drawCenterDots(QPainter &painter, const QPointF &center)
{
    painter.setPen(Qt::NoPen);

    QBrush WhiteBrush(Qt::white);
    painter.setBrush(WhiteBrush);
    painter.drawEllipse(center, 16, 16);

    QBrush GrayBrush(Qt::gray);
    painter.setBrush(GrayBrush);
    painter.drawEllipse(center, 12, 12);
}

void Canvas::drawNeedle(QPainter &painter, const QPointF &center)
{
    QPen needlePen(Qt::gray);
    needlePen.setWidth(8);
    painter.setPen(needlePen);

    double angle = 210 - (speed / 240.0) * 240;
    double radian = angle * M_PI / 180.0;

    int needleLength = 220;
    int needleTipX = center.x() + needleLength * std::cos(radian);
    int needleTipY = center.y() - needleLength * std::sin(radian);

    int needleBaseWidth = 12;
    QPointF baseLeft = QPointF(center.x() - needleBaseWidth * std::sin(radian),
                               center.y() - needleBaseWidth * std::cos(radian));
    QPointF baseRight = QPointF(center.x() + needleBaseWidth * std::sin(radian),
                                center.y() + needleBaseWidth * std::cos(radian));

    QPolygonF needlePolygon;
    needlePolygon << baseLeft << baseRight << QPointF(needleTipX, needleTipY);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));
    painter.drawPolygon(needlePolygon);
}

void Canvas::drawIcons(QPainter &painter, const QPointF &center)
{
    QFont largeIconFont("FontAwesome", 50);
    painter.setFont(largeIconFont);

    QPen iconPen(Qt::green);
    painter.setPen(iconPen);

    QRectF rectangle(center.x() - 250, center.y() - 250, 500, 500);

    if (leftArrowVisible)
        painter.drawText(QRectF(rectangle.left() - 100, rectangle.top() - 80, 100, 80), Qt::AlignCenter, QChar(0xe5c4));

    if (rightArrowVisible)
        painter.drawText(QRectF(rectangle.right(), rectangle.top() - 80, 100, 80), Qt::AlignCenter, QChar(0xe5c8));
}

void Canvas::drawThermostat(QPainter &painter, const QPointF &center)
{
    const int thermostatWidth = 150;  // Width of the thermostat icon
    const int thermostatHeight = 150; // Height of the thermostat icon
    const int horizontalOffset = 378; // Horizontal offset from the center
    const int verticalOffset = 180;   // Vertical offset from the center to lower the icon
    const int textOffsetY = -55;      // Adjusted vertical offset for text closer to the thermostat icon

    // Position of the thermostat icon
    QPointF thermostatCenter(center.x() + horizontalOffset, center.y() + verticalOffset - thermostatHeight / 2);

    QRectF thermostatRect(thermostatCenter.x() - thermostatWidth / 2, thermostatCenter.y() - thermostatHeight / 2, thermostatWidth, thermostatHeight);

    // Draw thermostat icon
    QPen iconPen;
    iconPen.setColor((temperature < 5) ? Qt::white : (temperature <= 39) ? Qt::blue
                                                                         : Qt::red);
    painter.setPen(iconPen);
    painter.setFont(QFont("FontAwesome", 50));
    painter.drawText(thermostatRect, Qt::AlignCenter, QChar(0xe1ff));

    // Draw temperature text below the thermostat icon
    QFont tempFont("Arial", 12);
    painter.setFont(tempFont);
    painter.setPen(Qt::white);
    QRectF tempTextRect(thermostatCenter.x() - thermostatWidth / 2, thermostatCenter.y() + thermostatHeight / 2 + textOffsetY, thermostatWidth, 40);
    painter.drawText(tempTextRect, Qt::AlignCenter, QString::number(temperature) + " °C");
}

void Canvas::drawConnectionStatus(QPainter &painter, const QPointF &center)
{
    QRectF rectangle(center.x() - 250, center.y() - 250, 500, 500);

    QFont font("FontAwesome", 50);
    painter.setFont(font);

    painter.setPen(isConnected ? Qt::white : Qt::red);
    painter.drawText(QRectF(center.x() - 40, rectangle.bottom() - 180, 80, 80), Qt::AlignCenter, QChar(0xe9e4));

    painter.setFont(QFont("Arial", isConnected ? 17 : 20));
    painter.drawText(QRectF(center.x() - 100, rectangle.bottom() - 120, 200, 40), Qt::AlignCenter, isConnected ? QString::number(speed) + " km/h" : "Connection Error");
}

void Canvas::drawBattery(QPainter &painter, const QPointF &center)
{
    const int batteryWidth = 150;     // Width of the battery icon
    const int batteryHeight = 300;    // Height of the battery icon
    const int horizontalOffset = 378; // Horizontal offset from the center
    const int verticalOffset = 150;   // Adjusted vertical offset to lower the battery icon
    const int textOffsetY = -125;     // Offset for text below the battery icon
    const int padding = 60;           // Padding to fit the level indicator

    // Calculate the center for the battery icon
    QPointF batteryCenter(center.x() + horizontalOffset, center.y() + verticalOffset - batteryHeight / 2);

    QRectF batteryRect(batteryCenter.x() - batteryWidth / 2, batteryCenter.y() - batteryHeight / 2, batteryWidth, batteryHeight);

    // Draw battery level text
    painter.setFont(QFont("Arial", 12));
    painter.setPen(Qt::white);
    QRectF batteryTextRect(batteryCenter.x() - batteryWidth / 2, batteryCenter.y() + batteryHeight / 2 + textOffsetY, batteryWidth, 40);
    painter.drawText(batteryTextRect, Qt::AlignCenter, QString::number(batteryLevel) + " %");

    // Draw battery icon
    QPen iconPen;
    iconPen.setColor((batteryLevel < 25) ? Qt::red : (batteryLevel <= 49) ? Qt::yellow
                                                                          : Qt::green);
    painter.setPen(iconPen);

    QFont batteryIconFont("FontAwesome", 70);
    painter.setFont(batteryIconFont);
    painter.drawText(batteryRect, Qt::AlignCenter, QChar(0xebdc));

    // Adjust the battery level rectangle to fit exactly within the battery icon's height
    int batteryLevelHeight = ((batteryHeight - 240) * batteryLevel) / 100; // Calculate the height based on battery level
    QRectF batteryLevelRect(batteryRect.left() + padding, batteryRect.top() + ((batteryHeight - 129) - batteryLevelHeight), batteryWidth - 2 * padding, batteryLevelHeight);
    painter.fillRect(batteryLevelRect, iconPen.color());
}

void Canvas::drawSpeedometer(QPainter &painter, const QPointF &center)
{
    drawSpeedometerArc(painter, center.toPoint(), ARC_WIDTH / 2);
    drawSpeedometerLines(painter, center.toPoint(), ARC_WIDTH / 2);
}

void Canvas::drawSpeedometerArc(QPainter &painter, QPoint center, int radius)
{
    QPen pen(Qt::white, 6);
    painter.setPen(pen);

    QFont font("Arial", 14);
    painter.setFont(font);

    for (int i = 0; i <= 12; ++i)
    {
        int speed = i * 20;
        double angle = 210 - (i / 12.0) * 240;
        double radian = angle * M_PI / 180.0;
        int x = center.x() + (radius - 45) * std::cos(radian);
        int y = center.y() - (radius - 45) * std::sin(radian);
        painter.drawText(QRect(x - 15, y - 10, 30, 20), Qt::AlignCenter, QString::number(speed));
    }
}

void Canvas::drawSpeedometerLines(QPainter &painter, QPoint center, int radius)
{
    QPen pen(Qt::white, 4);
    painter.setPen(pen);

    for (int i = 0; i <= 12; ++i)
    {
        double angle = 210 - (i / 12.0) * 240;
        double radian = angle * M_PI / 180.0;

        int xStart = center.x() + (radius - 25) * std::cos(radian);
        int yStart = center.y() - (radius - 25) * std::sin(radian);
        int xEnd = center.x() + (radius - 10) * std::cos(radian);
        int yEnd = center.y() - (radius - 10) * std::sin(radian);

        painter.drawLine(xStart, yStart, xEnd, yEnd);
    }

    QPen smallPen(Qt::white, 2);
    QPen boldPen(Qt::white, 4);
    painter.setPen(smallPen);

    for (int i = 0; i <= 48; ++i)
    {
        if (i % 4 != 0)
        {
            double angle = 210 - (i / 48.0) * 240;
            double radian = angle * M_PI / 180.0;

            int xStart = center.x() + (radius - 20) * std::cos(radian);
            int yStart = center.y() - (radius - 20) * std::sin(radian);
            int xEnd = center.x() + (radius - 10) * std::cos(radian);
            int yEnd = center.y() - (radius - 10) * std::sin(radian);

            painter.setPen(i % 2 == 0 ? boldPen : smallPen);
            painter.drawLine(xStart, yStart, xEnd, yEnd);
        }
    }
}
 "canvas.h"
