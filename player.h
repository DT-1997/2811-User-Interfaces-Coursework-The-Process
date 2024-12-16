#ifndef PLAYER_H
#define PLAYER_H

#include "ui_mainwindow.h"
#include "tomeo_ui.h"
#include "mySlider.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QListWidgetItem>
#include "button.h"
#include <QTimer.h>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>

// Structure to hold comment data including username, text, avatar, and timestamp
struct CommentData {
    QString username;     // Username of the commenter
    QString commentText;  // The text of the comment
    QString avatarPath;   // Path to the commenter's avatar image
    QString timestamp;    // Timestamp of when the comment was posted
};

// Player class inherited from QMediaPlayer to manage video playback and related UI actions
class Player : public QMediaPlayer {
    Q_OBJECT

public:
    // Constructor initializes the player and sets up connections for UI elements
    Player(Ui::MainWindow* ui, QWidget* parent)
        :   QMediaPlayer(parent),
        player_ui(ui),
        uiTool(ui),
        player(new QMediaPlayer(this)),
        playerList(new QMediaPlaylist),
        currentVideoIndex(0),
        currentPlaybackRate(1.0)
    {
        // Place the QVideoWidget in a new QWidget container for better layout control
        videoWidget = new QVideoWidget(player_ui->videoWidget); // Assign a new container for the video widget
        videoWidget->resize(player_ui->videoWidget->size()); // Resize video widget to match the container's size

        initComments(); // Initialize comments area and styles
        initData();

        // Set up a vertical layout to include the video widget and adjust layout margins
        QVBoxLayout *layout = new QVBoxLayout(player_ui->videoWidget);
        layout->addWidget(videoWidget);  // Add the video widget to the layout
        layout->setContentsMargins(0, 0, 0, 0);  // Remove margins to allow video to fill the container
        player_ui->videoWidget->setLayout(layout); // Set the layout for the video container

        // Set up playlist and output for media playback
        player->setPlaylist(playerList);
        player->setVideoOutput(videoWidget);

        // Update UI styles using helper functions
        uiTool.updateProgressBarStyle();  // Set the style for the progress bar
        uiTool.updateVolumeBarStyle();    // Set the style for the volume slider
        uiTool.setCommentAreaStyle(400);  // Set the style for the comment area
        uiTool.setVideoLabelStyle();      // Set the style for the video label

        uiTool.setButtonSize();           // Set the size of the buttons
        uiTool.loadIcon();                // Load the necessary icons
        uiTool.setButtonStyle();          // Apply styles to buttons

        // Connect button click events to the corresponding slots
        connect(player_ui->speedButton, &QPushButton::clicked, this, &Player::onSpeedButtonClicked);

        // Connect fast forward button click event
        connect(player_ui->fastForwardButton, &QPushButton::clicked, this, &Player::onFastForward);

        // Connect fast rewind button click event
        connect(player_ui->fastRewindButton, &QPushButton::clicked, this, &Player::onFastRewind);

        // Connect list widget item selection change to update video title
        connect(player_ui->listWidget, &QListWidget::itemSelectionChanged,
                this, &Player::updateVideoTitle);

        // Connect other button events for like, star, share, and send actions
        connect(player_ui->likeButton, &QPushButton::clicked, this, &Player::onLikeButtonCliked);
        connect(player_ui->starButton, &QPushButton::clicked, this, &Player::onStarButtonCliked);
        connect(player_ui->shareButton, &QPushButton::clicked, this, &Player::onShareButtonClicked);
        connect(player_ui->sendButton, &QPushButton::clicked, this, &Player::onSendButtonClicked);

        // Hide the comment section widgets initially
        for (int i = 0; i < player_ui->commentLayout->count(); ++i) {
            QWidget *widget = player_ui->commentLayout->itemAt(i)->widget();
            if (widget) {
                widget->hide();  // Hide comment widget
            }
            player_ui->closeCommentButton->hide();
            player_ui->sayLabel->hide();
        }

        // Set the initial volume to 10% and update the volume slider
        player->setVolume(10);  // Set initial volume level to 10%
        player_ui->volumeSlider->setValue(10);

        // Connect volume button to its respective slot
        connect(player_ui->volumeButton, &QPushButton::clicked, this, &Player::onVolumeButtonClicked);

        // Connect volume slider to change volume based on user interaction
        connect(player_ui->volumeSlider, &MySlider::costomSliderClicked, this, &Player::onVolumeSliderChanged);
        connect(player_ui->volumeSlider, &MySlider::sliderMoved, this, &Player::onVolumeSliderChanged);

        // Set default state to pause for the play/pause button
        // Connect the play/pause button to its respective slot
        connect(player_ui->playPauseButton, &QPushButton::clicked, this, &Player::togglePlayPause);

        // Connect next and previous buttons to their respective slots for video navigation
        connect(player_ui->nextButton, &QPushButton::clicked, this, &Player::playNextVideo);
        connect(player_ui->previousButton, &QPushButton::clicked, this, &Player::playPreviousVideo);

        // Connect the progress slider to adjust the video position
        connect(player_ui->progressSlider, &MySlider::costomSliderClicked, this, &Player::onProgressSliderClicked);
        connect(player_ui->progressSlider, &MySlider::sliderMoved, this, &Player::onProgressSliderMoved);

        // Connect slider released signal to update the video position once the slider is released
        connect(player_ui->progressSlider, &MySlider::sliderReleased, this, &Player::onProgressSliderReleased);

        // Set the range for the progress slider
        player_ui->progressSlider->setRange(0, maxValue);

        // Create a timer to regularly update the progress bar
        progressTimer = new QTimer(this);
        connect(progressTimer, &QTimer::timeout, this, &Player::onTimerOut);
        progressTimer->setInterval(20);  // Set the timer interval to 20ms
        progressTimer->start();          // Start the timer to periodically update the progress

        // Connect signals for media status changes and updating time display
        connect(player, &QMediaPlayer::mediaStatusChanged, this, &Player::onMediaStatusChanged);
        connect(player, &QMediaPlayer::positionChanged, this, &Player::updateTimeDisplay);
        connect(player, &QMediaPlayer::durationChanged, this, &Player::updateTimeDisplay);

        // Connect item click event in the video list widget to update the video
        connect(player_ui->listWidget, &QListWidget::itemPressed, this, &Player::onVideoItemClicked);


        // Retrieve command-line arguments for loading video folder
        QStringList arguments = QCoreApplication::arguments();

        if (arguments.size() == 2) {
            // If a folder path is provided as an argument, load the videos from that folder
            QString folderPath = arguments[1];  // Get the first command-line argument (folder path)
            loadVideosFromFolder(folderPath);    // Call the method to load videos
        } else {
            // If no folder path is provided, show an error message
            const int result = QMessageBox::information(
                NULL,
                QString("Tomeo"),
                QString("No videos found! Please add a command line argument with the folder location."));
            exit(-1);  // Exit the program if no folder is specified
        }
    }

    // Method to load videos from a specified folder
    void loadVideosFromFolder(const QString &folderPath);

    // Method to update the width of a child widget within a parent widget
    void updateChildWidgetWidth(QWidget *parentWidget, QWidget *childWidget, double percentage);

    // Private members of the Player class
private:
    Ui::MainWindow *player_ui;  // Pointer to the UI of the main window

    TomeoUi uiTool;  // Custom UI helper object for updating UI elements

    // Slot methods for handling button clicks
    void on_playButton_clicked();        // Slot to handle play button click
    void on_pauseButton_clicked();       // Slot to handle pause button click
    void on_previousButton_clicked();    // Slot to handle previous button click
    void on_nextButton_clicked();        // Slot to handle next button click
    void adjustPlayPause();

    // Method to animate a comment label (e.g., scrolling effect)
    void animateComment(QLabel *commentLabel);

    // Method to display a comment in the comment section
    void displayComment(QLabel* commentLabel, const QString& commentText);

    // Method to add a new comment with user info
    void addComment(const QString &username, const QString &commentText, const QString &avatarPath, const QString &timestamp);

    // Method to create a new comment widget from CommentData
    QWidget* createCommentWidget(const CommentData &data);

    // Initialize the comments section (e.g., set up layout, widgets, etc.)
    void initComments();

    void initData();

    QMediaPlayer* player;           // Media player object for video playback
    QMediaPlaylist* playerList;     // Playlist object to manage video list
    QVideoWidget* videoWidget;      // Video widget for displaying video
    QTimer* progressTimer;          // Timer for updating progress bar at regular intervals

    int currentVideoIndex;          // Index of the currently playing video in the playlist
    int maxValue = 10000;           // Maximum value for the progress slider
    int previousVolume;             // Stores the previous volume for toggling mute/unmute
    float currentPlaybackRate;      // Current playback speed (default is 1.0 for normal speed)

public slots:
    // Slot to handle the release of the progress slider
    void onProgressSliderReleased();

    // Slot to handle timer timeout event (used for updating progress)
    void onTimerOut();

    // Slot for fast-forward button action
    void onFastForward();

    // Slot for fast rewind button action
    void onFastRewind();

    // Slot to play the next video in the playlist
    void playNextVideo();

    // Slot to play the previous video in the playlist
    void playPreviousVideo();

    // Slot to toggle play/pause state
    void togglePlayPause();

    // Slot to set the video volume based on the slider value
    void setVolume(int volume);

    // Slot to update the time display on the UI
    void updateTimeDisplay();

    // Slot to handle item click event in the video list
    void onVideoItemClicked(QListWidgetItem *item);

    // Slot for handling progress slider click event
    void onProgressSliderClicked();

    // Slot for handling progress slider move event
    void onProgressSliderMoved();

    // Slot to handle media status change events (e.g., playing, paused, etc.)
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

    // Slot to handle changes in the volume slider value
    void onVolumeSliderChanged();

    // Slot for volume button click event
    void onVolumeButtonClicked();

    // Slot to handle like button click event
    void onLikeButtonCliked();

    // Slot to handle star button click event
    void onStarButtonCliked();

    // Slot to handle share button click event
    void onShareButtonClicked();

    // Slot to handle send button click event (for comments)
    void onSendButtonClicked();

    // Slot to update the video title based on the selected video in the list
    void updateVideoTitle();

    // Slot to handle speed button click event (to change playback speed)
    void onSpeedButtonClicked();

    // Method to get the current time of the video (used for time display)
    QString getCurrentTime();
};


#endif // PLAYER_H
