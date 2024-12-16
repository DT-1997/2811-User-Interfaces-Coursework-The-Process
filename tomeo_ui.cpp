#include "tomeo_ui.h"
#include "player.h"
#include <QPainter>
#include <QFontDatabase>
#include <QStyle>


void TomeoUi::updateProgressBarStyle()
{
    // Define the style for the progress bar
    QString styleSheet = QString(
        "QSlider::groove:horizontal {"
        "height: 12px;"  // Set the height of the progress bar
        "border-radius: 4px;"  // Set rounded corners
        "} "
        "QSlider::handle:horizontal {"
        "background: white;"  // Set the color of the slider handle
        "border: 2px solid gray;"  // Set the border color of the handle
        "width: 25px;"  // Set the width of the circular slider handle
        "height: 25px;"  // Set the height of the circular slider handle
        "border-radius: 8px;"  // Set the handle to be circular
        "margin-top: -4px;"  // Adjust the handle position to center it vertically
        "margin-bottom: -4px;"  // Adjust the handle position to center it vertically
        "} "
        "QSlider::sub-page:horizontal {"
        "background: #ff99cc;"  // Set the color for the filled portion of the progress bar
        "border-radius: 4px;"
        "height: 8px;"
        "} "
        "QSlider::add-page:horizontal {"
        "background: #333333;"  // Set the color for the unfilled portion of the progress bar
        "border-radius: 4px;"  // Set rounded corners
        "height: 8px;"  // Set the height of the progress bar
        "}"
        );

    tomeo_ui->progressSlider->setStyleSheet(styleSheet);  // Apply the style to the progress slider
}

void TomeoUi::updateVolumeBarStyle()
{
    // Define the style for the volume bar
    QString styleSheet = QString(
        "QSlider::groove:horizontal {"
        "height: 8px;"  // Set the height of the volume bar
        "border-radius: 4px;"  // Set rounded corners
        "} "
        "QSlider::handle:horizontal {"
        "background: pink;"  // Set the color of the slider handle
        "border: 2px solid #ffffff;"  // Set the border color of the handle
        "width: 15px;"  // Set the width of the circular slider handle
        "height: 15px;"  // Set the height of the circular slider handle
        "border-radius: 3px;"  // Set the handle to be circular
        "margin-top: -4px;"  // Adjust the handle position to center it vertically
        "margin-bottom: -4px;"  // Adjust the handle position to center it vertically
        "} "
        "QSlider::sub-page:horizontal {"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #FFFFFF, stop:0.7 #FFFFFF, stop:1 #DBDBDB);"  // Set the color gradient for the filled portion of the volume slider
        "border-radius: 4px;"
        "height: 8px;"
        "} "
        "QSlider::add-page:horizontal {"
        "background: #333333;"  // Set the color for the unfilled portion of the volume slider
        "border-radius: 4px;"  // Set rounded corners
        "height: 8px;"  // Set the height of the volume slider
        "}"
        );

    tomeo_ui->volumeSlider->setStyleSheet(styleSheet);  // Apply the style to the volume slider
}

void TomeoUi::setButtonSize(){
    // Set minimum and maximum sizes for buttons
    tomeo_ui->playPauseButton->setMinimumSize(80, 40);  // Set the minimum size of the play/pause button
    tomeo_ui->playPauseButton->setMaximumSize(150, 90); // Set the maximum size of the play/pause button

    tomeo_ui->previousButton->setMinimumSize(80, 40);  // Set the minimum size of the previous button
    tomeo_ui->previousButton->setMaximumSize(150, 90); // Set the maximum size of the previous button

    tomeo_ui->nextButton->setMinimumSize(80, 40);  // Set the minimum size of the next button
    tomeo_ui->nextButton->setMaximumSize(150, 90); // Set the maximum size of the next button

    tomeo_ui->volumeButton->setMinimumSize(80, 40);  // Set the minimum size of the volume button
    tomeo_ui->volumeButton->setMaximumSize(150, 90); // Set the maximum size of the volume button
}

void TomeoUi::loadIcon(){
    // Load custom icons from the font file
    int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");  // Load the font file
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);  // Get the font family name
    QFont actionIconFont = QFont(fontName);  // Create a font object for action icons
    QFont mediaIconFont = QFont(fontName);   // Create a font object for media icons
    QFont deviceIconFont = QFont(fontName);  // Create a font object for device icons
    QFont barIconFont = QFont(fontName);     // Create a font object for bar icons

    // Set pixel size for different icon fonts
    actionIconFont.setPixelSize(60);  // Set the size of the action icon font
    mediaIconFont.setPixelSize(40);   // Set the size of the media icon font
    deviceIconFont.setPixelSize(35);  // Set the size of the device icon font
    barIconFont.setPixelSize(50);     // Set the size of the bar icon font

    // Set the icon font and text for various buttons
    tomeo_ui->likeButton->setFont(actionIconFont);
    tomeo_ui->likeButton->setText(QString(QChar(0xe85c)));

    tomeo_ui->starButton->setFont(actionIconFont);
    tomeo_ui->starButton->setText(QChar(0xe646));

    tomeo_ui->commentButton->setFont(actionIconFont);
    tomeo_ui->commentButton->setText(QChar(0xe63a));

    tomeo_ui->closeCommentButton->setFont(deviceIconFont);
    tomeo_ui->closeCommentButton->setText(QChar(0xe60a));

    tomeo_ui->shareButton->setFont(actionIconFont);
    tomeo_ui->shareButton->setText(QChar(0xe629));

    tomeo_ui->fastForwardButton->setFont(mediaIconFont);
    tomeo_ui->fastForwardButton->setText(QChar(0xe6c6));

    tomeo_ui->fastRewindButton->setFont(mediaIconFont);
    tomeo_ui->fastRewindButton->setText(QChar(0xe6c5));

    tomeo_ui->previousButton->setFont(mediaIconFont);
    tomeo_ui->previousButton->setText(QChar(0xe63c));

    tomeo_ui->nextButton->setFont(mediaIconFont);
    tomeo_ui->nextButton->setText(QChar(0xe63e));

    tomeo_ui->playPauseButton->setFont(mediaIconFont);
    tomeo_ui->playPauseButton->setText(QChar(0xe628));

    tomeo_ui->volumeButton->setFont(mediaIconFont);
    tomeo_ui->volumeButton->setText(QChar(0xe609));

    tomeo_ui->phoneButton->setFont(deviceIconFont);
    tomeo_ui->phoneButton->setText(QChar(0xe645));

    tomeo_ui->padButton->setFont(deviceIconFont);
    tomeo_ui->padButton->setText(QChar(0xe654));

    tomeo_ui->desktopButton->setFont(mediaIconFont);
    tomeo_ui->desktopButton->setText(QChar(0xe6d3));

    tomeo_ui->showListButton->setFont(mediaIconFont);
    tomeo_ui->showListButton->setText(QChar(0x344a));

    tomeo_ui->fullScreenButton->setFont(barIconFont);
    tomeo_ui->fullScreenButton->setText(QChar(0xeb11));

    tomeo_ui->line1->setFont(barIconFont);
    tomeo_ui->line1->setText(QChar(0xe820));

    tomeo_ui->line2->setFont(barIconFont);
    tomeo_ui->line2->setText(QChar(0xe820));

    tomeo_ui->line3->setFont(barIconFont);
    tomeo_ui->line3->setText(QChar(0xe820));

    tomeo_ui->line4->setFont(barIconFont);
    tomeo_ui->line4->setText(QChar(0xe820));
}

void TomeoUi::setButtonStyle() {
    // Style for the 'like' button
    QString likeStyle = QString(
        "QPushButton {"
        "   color: lightgray;"                   // Set the text color of the button to light gray
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "}"
        "QPushButton:hover {"
        "   color: red;"                         // Text color changes to red when mouse hovers over the button
        "}"
        "QPushButton:pressed {"
        "   color: red;"                         // Text color changes to red when the button is pressed
        "}");

    // Style for the 'star' button
    QString starStyle = QString(
        "QPushButton {"
        "   color: lightgray;"                   // Set the text color of the button to light gray
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "}"
        "QPushButton:hover {"
        "   color: #fdb702;"                     // Text color changes to gold when mouse hovers over the button
        "}"
        "QPushButton:pressed {"
        "   color: #fdb702;"                     // Text color remains gold when the button is pressed
        "}");


    // Style for the 'comment' button
    QString commentStyle = QString(
        "QPushButton {"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "}");


    // Style for the 'share' button
    QString shareStyle = QString(
        "QPushButton {"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "}");


    // Style for the 'media' control buttons (play, pause, next, etc.)
    QString mediaStyle = QString(
        "QPushButton {"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "}");


    // Apply styles to buttons
    tomeo_ui->likeButton->setStyleSheet(likeStyle);
    tomeo_ui->starButton->setStyleSheet(starStyle);
    tomeo_ui->commentButton->setStyleSheet(commentStyle);
    tomeo_ui->closeCommentButton->setStyleSheet(commentStyle);
    tomeo_ui->shareButton->setStyleSheet(shareStyle);
    tomeo_ui->fastForwardButton->setStyleSheet(mediaStyle);
    tomeo_ui->fastRewindButton->setStyleSheet(mediaStyle);
    tomeo_ui->previousButton->setStyleSheet(mediaStyle);
    tomeo_ui->nextButton->setStyleSheet(mediaStyle);
    tomeo_ui->playPauseButton->setStyleSheet(mediaStyle);
    tomeo_ui->volumeButton->setStyleSheet(mediaStyle);
    tomeo_ui->phoneButton->setStyleSheet(mediaStyle);
    tomeo_ui->padButton->setStyleSheet(mediaStyle);
    tomeo_ui->desktopButton->setStyleSheet(mediaStyle);
    tomeo_ui->showListButton->setStyleSheet(mediaStyle);
    tomeo_ui->fullScreenButton->setStyleSheet(mediaStyle);
    tomeo_ui->line1->setStyleSheet(mediaStyle);
    tomeo_ui->line2->setStyleSheet(mediaStyle);
    tomeo_ui->line3->setStyleSheet(mediaStyle);
    tomeo_ui->line4->setStyleSheet(mediaStyle);
}

void TomeoUi::setCommentAreaStyle(int size)
{
    // Style for the comment area (textarea)
    tomeo_ui->commentArea->setStyleSheet(
        "QTextEdit#commentArea {"
        "    background-color: #f7f7f7;"           /* Light gray background color for the comment area */
        "    border: 1px solid #e1e1e1;"           /* Light gray border */
        "    border-radius: 15px;"                 /* Rounded corners */
        "    padding: 10px;"                       /* Padding inside the comment area */
        "    color: #333333;"                      /* Text color set to dark gray */
        "    font-size: 24px;"                     /* Font size for the text */
        "    font-family: 'Comic Sans MS', sans-serif;" /* Font family */
        "    max-height: 180px;"                   /* Maximum height to prevent the comment box from expanding infinitely */
        "    min-height: 100px;"                   /* Minimum height to ensure the comment box has space for input */
        "    line-height: 1.5;"                    /* Line spacing for readability */
        "}"

        "QTextEdit#commentArea:focus {"
        "    border: 5px solid #ff66cc;"           /* Pink border when the comment area is focused */
        "    background-color: #fff;"              /* White background when focused */
        "}"

        "QTextEdit#commentArea::placeholder {"
        "    color: #999999;"                      /* Light gray color for placeholder text */
        "    font-style: italic;"                  /* Italic style for placeholder text */
        "}"
        );

    // Set spacing between comment area elements to 0
    tomeo_ui->commentLayout->setSpacing(0);

    // Set style for the label above the comment area
    tomeo_ui->sayLabel->setStyleSheet(
        "QLabel#sayLabel {"
        "    color: white;"                         /* Set text color to white */
        "    font-size: 24px;"                      /* Set font size */
        "}"
        );

    // Set fixed width for the comment area, list, and send button
    tomeo_ui->commentList->setFixedWidth(size);  // Set minimum width for the comment list
    tomeo_ui->commentArea->setFixedWidth(size);  // Set minimum width for the comment area
    tomeo_ui->sendButton->setFixedWidth(size);   // Set minimum width for the send button
}

void TomeoUi::setVideoLabelStyle()
{
    // Set style for the video label
    tomeo_ui->videoLabel->setStyleSheet(
        "font-family: 'Comic Sans MS';"         // Set font family
        "color: white;"                         // Set font color to white
        "font-size: 40px;"                      // Set font size
        "font-weight: bold;"                    // Make the font bold
        "background-color: transparent;"       // Set background to transparent
        "border: none;"                         // No border around the label
        "padding: 5px;"                         // Add padding around the text
        "text-align: center;"                   // Center the text horizontally
        );
}
