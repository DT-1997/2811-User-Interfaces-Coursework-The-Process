#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Initialize the Qt application
    MainWindow w;  // Create the main window object

    // Get the screen size (available geometry of the screen)
    QDesktopWidget* desktop = QApplication::desktop();  // Get the desktop widget to access screen properties
    QRect screenRect = desktop->availableGeometry(); // Get the available geometry of the screen
    int screenWidth = screenRect.width();  // Width of the available screen space
    int screenHeight = screenRect.height();  // Height of the available screen space

    // Get the main window's current size
    int windowWidth = w.width();  // Width of the main window
    int windowHeight = w.height();  // Height of the main window

    // Calculate the position where the window should be placed (centered on the screen)
    int x = (screenWidth - windowWidth) / 2;  // X-coordinate to center the window horizontally
    int y = (screenHeight - windowHeight) / 2;  // Y-coordinate to center the window vertically

    // Move the main window to the calculated position (center of the screen)
    w.move(x, y);

    // Show the main window
    w.show();

    // Start the event loop of the application
    return a.exec();
}
