#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "player.h"
#include <QApplication>
#include <QScreen>
#include <QFontDatabase>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    uiTool(ui)  // Initialize the UI and UI tools
{
    ui->setupUi(this);  // Setup the UI elements from the designer
    player = new Player(ui, this);  // Initialize the Player object with the UI

    // Initialize the mouse movement timer to trigger every 50 milliseconds
    mouseMoveTimer = new QTimer(this);
    mouseMoveTimer->setInterval(50);  // Set the timer interval to 50 milliseconds
    connect(mouseMoveTimer, &QTimer::timeout, this, &MainWindow::updateMousePosition);  // Connect timer to update mouse position

    // Connect button click signals to the corresponding slots
    connect(ui->commentButton, &QPushButton::clicked, this, &MainWindow::onCommentButtonClicked);
    connect(ui->closeCommentButton, &QPushButton::clicked, this, &MainWindow::onCommentButtonClicked);

    // Connect various buttons for device mode and layout adjustment
    connect(ui->phoneButton, &QPushButton::clicked, this, &MainWindow::onPhoneResolutionClicked);
    connect(ui->padButton, &QPushButton::clicked, this, &MainWindow::onTabletResolutionClicked);
    connect(ui->desktopButton, &QPushButton::clicked, this, &MainWindow::onDesktopResolutionClicked);
    connect(ui->showListButton, &QPushButton::clicked, this, &MainWindow::onShowListButtonClicked);
    connect(ui->fullScreenButton, &QPushButton::clicked, this, &MainWindow::onFullscreenButtonClicked);

    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::updateWatchLabel);
    connect(ui->previousButton, &QPushButton::clicked, this, &MainWindow::updateWatchLabel);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updateWatchLabel);

    updateDeviceMode();  // Update the device mode based on user selection

    setWindowTitle("Tomeo");  // Set the window title

    // Get screen width and height
    screenHeight = getScreenHeight();
    screenWidth = getScreenWidth();

    // Set the initial window size to 85% of screen width and 70% of screen height
    this->resize(screenWidth * 0.85, screenHeight * 0.7);

    // Disable maximize button in window title bar
    this->setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    // Enable mouse tracking for the main window and its central widget
    this->setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);

    // Set minimum window size to 30% of screen width and 50% of screen height
    this->setMinimumSize(screenWidth * 0.3, screenHeight * 0.5);

    // Adjust the width of the video list based on a percentage of the central widget's width
    player->updateChildWidgetWidth(ui->centralwidget, ui->listWidget, 4);

    // Apply custom stylesheet for the UI elements (ListWidget, ScrollBar, Buttons, etc.)
    setStyleSheet("QListWidget {"
                  "   border: none;"  // Remove border from ListWidget
                  "   padding: 0;"  // Set padding to 0
                  "   background: transparent;"  // Set background to transparent
                  " }"
                  "QListWidget::item {"
                  "   padding: 15px 10px;"  // Set padding for items (15px top/bottom, 10px left/right)
                  "   border-radius: 15px;"  // Set rounded corners for items
                  "} "
                  "QListWidget::item:selected {"
                  "   background-color: #33343f;"  // Set background color for selected item
                  "   color: white;"  // Set text color to white for selected item
                  "}"
                  "QScrollBar:vertical {"
                  "   width: 15px;"  // Set the width of the vertical scrollbar
                  "   background: transparent;"  // Set scrollbar background to transparent
                  "   border-radius: 4px;"  // Set scrollbar corners to be rounded
                  " }"
                  "QScrollBar::handle:vertical {"
                  "   background: #555555;"  // Set scrollbar handle color
                  "   border-radius: 4px;"  // Set rounded corners for scrollbar handle
                  "   min-height: 20px;"  // Set minimum height for the scrollbar handle
                  " }"
                  "QScrollBar::handle:vertical:hover {"
                  "   background: #888888;"  // Set scrollbar handle color on hover
                  " }"
                  "QScrollBar::handle:vertical:pressed {"
                  "   background: #444444;"  // Set scrollbar handle color when pressed
                  " }"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                  "   background: transparent;"  // Set up/down arrow background to transparent
                  "   height: 0px;"  // Remove the height for up/down arrows
                  " }"
                  "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                  "   background: transparent;"  // Remove up/down arrow visuals
                  "   height: 0px;"  // Remove height for arrows
                  " }"
                  "QScrollBar::corner {"
                  "   background: transparent;"  // Set corner background to transparent
                  " }"
                  "QMainWindow {"
                  "   background-color: #17181b;"  // Set main window background color
                  " }"
                  "QListWidget#commentList {"
                  "    margin-bottom: 20px;"  // Add 20px margin to the bottom of the comment list
                  "}"
                  "QTextEdit#commentArea {"
                  "    margin-bottom: 20px;"  // Add 20px margin to the bottom of the comment area
                  "}"
                  "QPushButton#speedButton {"
                  "font-family: 'Comic Sans MS';"  // Set font family for speed button
                  "color: white;"  // Set font color to white
                  "font-size: 35px;"  // Set font size
                  "font-weight: bold;"  // Make font bold
                  "background-color: transparent;"  // Set background color to transparent
                  "border: none;"  // Remove border from button
                  "}"
                  "QLabel#likeCount {"
                  "font-family: 'Comic Sans MS';"  // Set font family for speed button
                  "color: white;"  // Set font color to white
                  "font-size: 25px;"  // Set font size
                  "font-weight: bold;"  // Make font bold
                  "background-color: transparent;"  // Set background color to transparent
                  "border: none;"  // Remove border from button
                  "}"
                  "QLabel#starCount {"
                  "font-family: 'Comic Sans MS';"  // Set font family for speed button
                  "color: white;"  // Set font color to white
                  "font-size: 25px;"  // Set font size
                  "font-weight: bold;"  // Make font bold
                  "background-color: transparent;"  // Set background color to transparent
                  "border: none;"  // Remove border from button
                  "}"
                  "QLabel#commentCount {"
                  "font-family: 'Comic Sans MS';"  // Set font family for speed button
                  "color: white;"  // Set font color to white
                  "font-size: 25px;"  // Set font size
                  "font-weight: bold;"  // Make font bold
                  "background-color: transparent;"  // Set background color to transparent
                  "border: none;"  // Remove border from button
                  "}"
                  );
}

MainWindow::~MainWindow()
{
    delete ui;  // Delete the UI object when the window is destroyed
}

int MainWindow::getScreenWidth() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    int screenWidth = screenRect.width();  // Get the screen width

    return screenWidth;
}

int MainWindow::getScreenHeight() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    int screenHeight = screenRect.height();  // Get the screen height

    return screenHeight;
}

void MainWindow::onPhoneResolutionClicked()
{
    int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");  // Load the font file
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);  // Get the font family name
    QFont watchIconFont = QFont(fontName);  // Create a font object for action icons
    watchIconFont.setPixelSize(15);

    ui->watchIcon->setFont(watchIconFont);

    screenHeight = getScreenHeight();
    screenWidth = getScreenWidth();
    // Set phone resolution, typically smaller window for phones
    this->resize(screenWidth * 0.25, screenHeight * 0.75);  // Set resolution for portrait mobile view

    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);  // Center the window

    // Set the style for the video label
    ui->videoLabel->setStyleSheet(
        "font-family: 'Comic Sans MS';"
        "color: white;"                 // Font color
        "font-size: 16px;"              // Font size
        "font-weight: bold;"            // Bold font
        "background-color: transparent;" // Transparent background
        "border: none;"                 // No border
        "padding: 5px;"                 // Padding
        "text-align: center;"           // Center the text
        );

    ui->watchLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Comic Sans MS';"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "   font-size: 15px;"
        "}");
}

void MainWindow::onTabletResolutionClicked()
{
    screenHeight = getScreenHeight();
    screenWidth = getScreenWidth();
    // Set tablet resolution, slightly larger window for tablets
    this->resize(screenWidth * 0.5, screenHeight * 0.5);  // Set resolution for tablet view

    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);  // Center the window
}

void MainWindow::onDesktopResolutionClicked()
{
    int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");  // Load the font file
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);  // Get the font family name
    QFont watchIconFont = QFont(fontName);  // Create a font object for action icons
    watchIconFont.setPixelSize(30);

    ui->watchIcon->setFont(watchIconFont);

    screenHeight = getScreenHeight();
    screenWidth = getScreenWidth();
    // Set desktop resolution, larger window for desktop screens
    this->resize(screenWidth * 0.85, screenHeight * 0.7);  // Set resolution for desktop view

    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);  // Center the window

    // Set the style for the video label
    ui->videoLabel->setStyleSheet(
        "font-family: 'Comic Sans MS';"
        "color: white;"                 // Font color
        "font-size: 40px;"              // Font size
        "font-weight: bold;"            // Bold font
        "background-color: transparent;" // Transparent background
        "border: none;"                 // No border
        "padding: 5px;"                 // Padding
        "text-align: center;"           // Center the text
        );

    ui->watchLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Comic Sans MS';"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "   font-size: 30px;"
        "}");
}

bool listState = false;

void MainWindow::onShowListButtonClicked()
{
    // Toggle the visibility of the video list
    if (ui->listWidget->isHidden()) {
        ui->listWidget->show();  // Show list
    } else {
        ui->listWidget->hide();  // Hide list
    }

    listState = !listState;  // Update the list state

    QSize windowSize = this->size();
    int windowWidth = windowSize.width();

    // If the window width is smaller than a threshold, hide comments and the progress bar
    if(windowWidth <= screenWidth * 0.3) {
        for (int i = 0; i < ui->commentLayout->count(); ++i) {
            QWidget *widget = ui->commentLayout->itemAt(i)->widget();
            if (widget) {
                widget->hide();  // Hide each comment widget
            }
        }
        ui->closeCommentButton->hide();
        ui->sayLabel->hide();
        if (listState) {
            showProgressBar(false);  // Hide the progress bar
        } else {
            showProgressBar(true);  // Show the progress bar
        }
    }
}

bool commentState = false;

void MainWindow::onCommentButtonClicked()
{
    showProgressBar(true);  // Show the progress bar

    // Toggle the visibility of the comment section
    if (commentState) {
        for (int i = 0; i < ui->commentLayout->count(); ++i) {
            QWidget *widget = ui->commentLayout->itemAt(i)->widget();
            if (widget) {
                widget->hide();  // Hide the comment widget
            }
        }
        ui->closeCommentButton->hide();
        ui->sayLabel->hide();
    } else {
        for (int i = 0; i < ui->commentLayout->count(); ++i) {
            QWidget *widget = ui->commentLayout->itemAt(i)->widget();
            if (widget) {
                widget->show();  // Show the comment widget
            }
        }
        ui->closeCommentButton->show();
        ui->sayLabel->show();
    }

    QSize windowSize = this->size();
    int windowWidth = windowSize.width();

    // Hide video list if the window is too small
    if(windowWidth <= screenWidth * 0.3) {
        ui->listWidget->hide();
    }

    commentState = !commentState;  // Update the comment section state
}

void MainWindow::onFullscreenButtonClicked()
{
    if (isFullScreen()) {
        showNormal();  // Restore the window from full screen
        ui->fullScreenButton->setText(QChar(0xeb11));  // Change the button text
        showControls(true);  // Show all controls
        mouseMoveTimer->stop();  // Stop the mouse move timer
    } else {
        showFullScreen();  // Switch to full screen
        ui->fullScreenButton->setText(QChar(0xeb10));  // Change the button text
        showControls(false);  // Hide all controls
        mouseMoveTimer->start();  // Start the mouse move timer
    }
    show();  // Update window display
}

void MainWindow::updateMousePosition()
{
    if (isFullScreen()) {
        QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
        int screenHeight = screenGeometry.height();

        // Get the mouse position
        QPoint mousePos = QCursor::pos();
        int mouseY = mousePos.y();

        qDebug() << "Mouse moved to:" << mouseY;

        if (mouseY >= screenHeight * 0.85) {  // If the mouse is in the bottom 10% of the screen
            showProgressBar(true);  // Show controls
        } else {
            showControls(false);  // Hide controls
        }
    }
}

void MainWindow::showControls(bool showAll)
{
    // Toggle visibility of various controls
    ui->listWidget->setVisible(showAll);
    ui->progressSlider->setVisible(showAll);  // Hide video list
    ui->durationLabel->setVisible(showAll);
    ui->currentTimeLabel->setVisible(showAll);
    ui->fastForwardButton->setVisible(showAll);
    ui->fastRewindButton->setVisible(showAll);
    ui->speedButton->setVisible(showAll);
    ui->nextButton->setVisible(showAll);
    ui->playPauseButton->setVisible(showAll);
    ui->previousButton->setVisible(showAll);
    ui->volumeButton->setVisible(showAll);
    ui->volumeSlider->setVisible(showAll);
    ui->phoneButton->setVisible(showAll);
    ui->padButton->setVisible(showAll);
    ui->desktopButton->setVisible(showAll);
    ui->line1->setVisible(showAll);
    ui->line2->setVisible(showAll);
    ui->line3->setVisible(showAll);
    ui->line4->setVisible(showAll);
}

void MainWindow::showProgressBar(bool showAll)
{
    // Toggle visibility of the progress bar controls
    ui->progressSlider->setVisible(showAll);
    ui->durationLabel->setVisible(showAll);
    ui->currentTimeLabel->setVisible(showAll);
    ui->fastForwardButton->setVisible(showAll);
    ui->fastRewindButton->setVisible(showAll);
    ui->speedButton->setVisible(showAll);
    ui->nextButton->setVisible(showAll);
    ui->playPauseButton->setVisible(showAll);
    ui->previousButton->setVisible(showAll);
    ui->volumeButton->setVisible(showAll);
    ui->volumeSlider->setVisible(showAll);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // Update the device mode whenever the window size changes
    updateDeviceMode();
}

void MainWindow::updateDeviceMode()
{
    // Get the current window size and determine the device mode
    QSize windowSize = this->size();
    int windowWidth = windowSize.width();

    // Switch between mobile, tablet, and desktop modes based on window width
    if (windowWidth >= screenWidth * 0.4 && windowWidth <= screenWidth * 0.51) {  // If the window width is small, treat it as mobile/tablet mode
        ui->listWidget->hide();  // Hide the right-side list widget
        ui->showListButton->show();  // Show the button to show list
        ui->line3->show();
        uiTool.setCommentAreaStyle(350);
        isPhone = false;

        ui->videoLabel->setStyleSheet(
            "font-family: 'Comic Sans MS';"
            "color: white;"                 // Font color
            "font-size: 30px;"              // Font size
            "font-weight: bold;"            // Bold font
            "background-color: transparent;" // Transparent background
            "border: none;"                 // No border
            "padding: 5px;"                 // Padding
            "text-align: center;"           // Center the text
            );

    } else if (windowWidth < screenWidth * 0.4 && windowWidth >= screenWidth * 0.01) {
        isPhone = true;
        ui->listWidget->hide();  // Hide the right-side list widget
        ui->showListButton->show();  // Show the button to show list
        ui->line3->show();
        uiTool.setCommentAreaStyle(350);
        int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");  // Load the font file
        QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);  // Get the font family name
        QFont watchIconFont = QFont(fontName);  // Create a font object for action icons
        watchIconFont.setPixelSize(15);

        ui->watchIcon->setFont(watchIconFont);

        // Set the style for the video label
        ui->videoLabel->setStyleSheet(
            "font-family: 'Comic Sans MS';"
            "color: white;"                 // Font color
            "font-size: 16px;"              // Font size
            "font-weight: bold;"            // Bold font
            "background-color: transparent;" // Transparent background
            "border: none;"                 // No border
            "padding: 5px;"                 // Padding
            "text-align: center;"           // Center the text
            );

        ui->watchLabel->setStyleSheet(
            "QLabel {"
            "   font-family: 'Comic Sans MS';"
            "   color: white;"                       // Set the text color of the button to white
            "   background-color: transparent;"      // Set the background color to transparent
            "   border: none;"                       // No border around the button
            "   font-size: 15px;"
            "}");
    }
    else {
        ui->listWidget->show();  // Show the right-side list widget
        ui->showListButton->hide();  // Hide the button to show list
        ui->line3->hide();
        uiTool.setCommentAreaStyle(400);
        isPhone = false;
    }
}

void MainWindow::updateWatchLabel()
{
    if (isPhone) {
        ui->videoLabel->setStyleSheet(
            "font-family: 'Comic Sans MS';"
            "color: white;"                 // Font color
            "font-size: 16px;"              // Font size
            "font-weight: bold;"            // Bold font
            "background-color: transparent;" // Transparent background
            "border: none;"                 // No border
            "padding: 5px;"                 // Padding
            "text-align: center;"           // Center the text
            );

        ui->watchLabel->setStyleSheet(
            "QLabel {"
            "   font-family: 'Comic Sans MS';"
            "   color: white;"                       // Set the text color of the button to white
            "   background-color: transparent;"      // Set the background color to transparent
            "   border: none;"                       // No border around the button
            "   font-size: 15px;"
            "}");
    } else {
        // Set the style for the video label
        ui->videoLabel->setStyleSheet(
            "font-family: 'Comic Sans MS';"
            "color: white;"                 // Font color
            "font-size: 40px;"              // Font size
            "font-weight: bold;"            // Bold font
            "background-color: transparent;" // Transparent background
            "border: none;"                 // No border
            "padding: 5px;"                 // Padding
            "text-align: center;"           // Center the text
            );

        ui->watchLabel->setStyleSheet(
            "QLabel {"
            "   font-family: 'Comic Sans MS';"
            "   color: white;"                       // Set the text color of the button to white
            "   background-color: transparent;"      // Set the background color to transparent
            "   border: none;"                       // No border around the button
            "   font-size: 30px;"
            "}");
    }
}

