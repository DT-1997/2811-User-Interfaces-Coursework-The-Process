#ifndef TOMEO_UI_H
#define TOMEO_UI_H

#include "ui_mainwindow.h"
#include <QStyle>
#include <QIcon>
#include <QApplication>

// The TomeoUi class is used for updating and managing the UI components' styles and icons.
class TomeoUi
{
public:
    // Constructor takes a pointer to the main window UI
    TomeoUi(Ui::MainWindow* ui) : tomeo_ui(ui) {}

    // Method to update the style of the progress bar
    void updateProgressBarStyle();

    // Method to update the style of the volume bar
    void updateVolumeBarStyle();

    // Method to set the style for the comment area
    void setCommentAreaStyle(int size);

    // Method to set the style of the buttons
    void setButtonStyle();

    // Method to set the size of the buttons
    void setButtonSize();

    // Method to set the style of the video label
    void setVideoLabelStyle();

    // Method to load and set icons for different UI components
    void loadIcon();

    // Stores the current application style
    QStyle* style;

    // Icon for the play/pause button
    QIcon pauseIcon;  // Icon for the pause state
    QIcon playIcon;   // Icon for the play state
    QIcon prevIcon;   // Icon for the previous video button
    QIcon nextIcon;   // Icon for the next video button
    QIcon volumeIcon; // Icon for the volume button
    QIcon volumeMutedIcon; // Icon for the muted volume button
    QIcon likeIcon;   // Icon for the like button
    QIcon starIcon;   // Icon for the star button
    QIcon shareIcon;  // Icon for the share button
    QIcon commentIcon; // Icon for the comment button
    QIcon sendIcon;   // Icon for the send button

private:
    Ui::MainWindow *tomeo_ui; // Pointer to the main window UI, used to access and update UI components
};

#endif // TOMEO_UI_H
