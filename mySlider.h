#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QCoreApplication>

// MySlider class inherits from QSlider and customizes the mouse press event handling
class MySlider : public QSlider
{
    Q_OBJECT

public:
    // Constructor: Initializes the custom slider
    MySlider(QWidget *parent = 0) : QSlider(parent)
    {}

protected:
    // Override mousePressEvent to capture mouse press events on the slider
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    // Custom signal to notify when the slider is clicked by the user
    void costomSliderClicked();  // This signal is emitted when the slider is clicked

};

#endif // MYSLIDER_H
