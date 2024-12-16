#include "mySlider.h"

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    // First, call the parent class's mouse press event to ensure normal slider behavior,
    // so that the dragging functionality is not affected.
    QSlider::mousePressEvent(ev);

    // Get the mouse position. We can't directly get it from the event (ev)
    // because the starting position of the mouse is not useful when dragging.
    double pos = ev->pos().x() / (double)width();

    // Calculate the new slider value based on the mouse position, then set the slider's value
    setValue(pos * (maximum() - minimum()) + minimum());

    // Emit a custom signal when the slider is clicked
    emit costomSliderClicked();
}
