#include "player.h"
#include "button.h"
#include "mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QDirIterator>
#include <QImageReader>
#include <QPropertyAnimation>
#include <QMediaMetaData>
#include <QFontDatabase>
#include <QRandomGenerator>
#include <QScrollBar>


// Function to read video files and thumbnails from the specified directory
std::vector<TheButtonInfo> getInfoIn(std::string loc) {

    std::vector<TheButtonInfo> out = std::vector<TheButtonInfo>();  // Vector to store video info
    QDir dir(QString::fromStdString(loc));  // Open the directory
    QDirIterator it(dir);  // Create an iterator to loop through files in the directory

    QString defaultThumb = ":/default.png";  // Path to the default thumbnail image (resource path or file path)

    while (it.hasNext()) { // Iterate through all files in the directory

        QString f = it.next();  // Get the current file

        if (f.contains("."))  // Check if the file has an extension (not a directory)

#if defined(_WIN32)
            if (f.contains(".wmv") || f.contains(".mp4") || f.contains(".MOV")) {  // Windows platforms
#else
            if (f.contains(".wmv") || f.contains(".mp4") || f.contains(".MOV")) {  // macOS/Linux platforms
#endif

                // Construct the expected thumbnail filename by replacing the video file extension with .png
                QString thumb = f.left(f.length() - 4) + ".png";
                QImage sprite;

                // Check if the thumbnail file exists and is valid
                if (QFile(thumb).exists()) {
                    QImageReader imageReader(thumb);
                    sprite = imageReader.read();  // Read the image data
                    if (sprite.isNull()) {
                        qDebug() << "Warning: Couldn't process thumbnail " << thumb << ", using default thumbnail.";
                        sprite = QImage(defaultThumb);  // Load the default thumbnail if the image is invalid
                    }
                } else {
                    qDebug() << "Warning: Thumbnail not found for " << f << ", using default thumbnail.";
                    sprite = QImage(defaultThumb);  // Use the default thumbnail if the file doesn't exist
                }

                // Create an icon from the image for the button
                QIcon* ico = new QIcon(QPixmap::fromImage(sprite));
                QUrl* url = new QUrl(QUrl::fromLocalFile(f));  // Convert the file path to a URL
                out.push_back(TheButtonInfo(url, ico));  // Add the video and thumbnail info to the output list
            }
    }

    return out;  // Return the list of video info with URLs and icons
}


// Function to load videos from a folder and populate the player UI with thumbnails and titles
void Player::loadVideosFromFolder(const QString &folderPath)
{
    QDir dir(folderPath);
    if (!dir.exists()) {  // Check if the directory exists
        qDebug() << "Directory does not exist";
        const int result = QMessageBox::information(
            NULL,
            QString("Tomeo"),
            QString("Directory does not exist! Add command line argument to \"quoted\" file location."));
        exit(-1);  // Exit the program with error code -1 if the directory is invalid
    }

    // Get all video files (e.g., mp4 format) from the directory
    QStringList filters;
    filters << "*.mp4" << "*.avi" << "*.mkv" << "*.MOV" << "*.wmv";  // Add more formats as needed
    QStringList videoFiles = dir.entryList(filters, QDir::Files);

    // If no video files were found, show a message and exit the program
    if (videoFiles.isEmpty()) {
        const int result = QMessageBox::information(
            NULL,
            QString("Tomeo"),
            QString("No videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);  // Exit the program with error code -1
    }

    // Get video information, including URLs and thumbnails
    std::vector<TheButtonInfo> videos = getInfoIn(folderPath.toStdString());

    playerList->clear();
    player_ui->listWidget->clear();  // Clear the existing list widget items

    // Add the video files to the media player playlist
    foreach (const QString &fileName, videoFiles) {
        QUrl url = QUrl::fromLocalFile(dir.filePath(fileName));  // Get the full file path as URL
        playerList->addMedia(url);  // Add the video to the playlist
    }

    // Set the size for each ListWidget item
    int itemHeight = 120;  // Set item height for each video
    player_ui->listWidget->setIconSize(QSize(200, 200));  // Set the size of the video thumbnail
    player_ui->listWidget->setStyleSheet("QListWidget::item { height: " + QString::number(itemHeight) + "px; }");  // Set the height of list items

    // Set up font for the view count icon
    int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont viewIconFont = QFont(fontName);
    viewIconFont.setPixelSize(30);  // Set font size for the view count icon

    // Loop through each video to create the list widget items with thumbnails and titles
    for (const TheButtonInfo &videoInfo : videos) {
        // Create a new QListWidgetItem
        QListWidgetItem *item = new QListWidgetItem();

        // Create a QWidget to contain the custom item layout
        QWidget *itemWidget = new QWidget(player_ui->listWidget);

        // Create the outer horizontal layout for the item
        QHBoxLayout *outerLayout = new QHBoxLayout();

        // Create QLabel to display the video thumbnail
        QLabel *thumbnailLabel = new QLabel(itemWidget);
        thumbnailLabel->setPixmap(videoInfo.icon->pixmap(150, 120));  // Set the thumbnail image
        thumbnailLabel->setFixedSize(150, 120);  // Set fixed size for the thumbnail
        thumbnailLabel->setStyleSheet("border: none; padding: 0;");  // Remove borders and padding

        // Create vertical layout for the video title and view count
        QVBoxLayout *textLayout = new QVBoxLayout();

        // Create QLabel to display the video title
        QLabel *titleLabel = new QLabel(videoInfo.url->fileName(), itemWidget);
        QFont titleFont("Comic Sans MS", 12, QFont::Bold);  // Set font for the title
        titleLabel->setFont(titleFont);
        titleLabel->setStyleSheet("color: white;");  // Set the font color of the title
        titleLabel->setWordWrap(true);  // Allow title to wrap if it's too long

        // Create a horizontal layout to display the view count icon and label
        QHBoxLayout *viewCountLayout = new QHBoxLayout();

        // Create QLabel for the view count icon
        QLabel *viewIconLabel = new QLabel(itemWidget);
        viewIconLabel->setText(QChar(0xe603));  // Set the view count icon (using Unicode character)
        viewIconLabel->setFont(viewIconFont);
        viewIconLabel->setStyleSheet("color: pink;");  // Set color for the view count icon

        // Create QLabel for the view count text (using random values for demonstration)
        int randomViews = QRandomGenerator::global()->bounded(1000, 10000);  // Random view count
        QLabel *viewCountLabel = new QLabel("Views: " + QString::number(randomViews), itemWidget);
        QFont viewCountFont("Comic Sans MS", 8);  // Set font for the view count text
        viewCountLabel->setFont(viewCountFont);
        viewCountLabel->setStyleSheet("color: #D3D3D3;");  // Set the font color for the view count

        // Add the view count icon and label to the horizontal layout
        viewCountLayout->addWidget(viewIconLabel);
        viewCountLayout->addWidget(viewCountLabel);

        // Add the title label and view count layout to the vertical layout
        textLayout->addWidget(titleLabel);
        textLayout->addLayout(viewCountLayout);  // Add the view count layout

        // Add the thumbnail label and text layout to the outer layout
        outerLayout->addWidget(thumbnailLabel);
        outerLayout->addLayout(textLayout);

        // Set the layout of the item widget
        itemWidget->setLayout(outerLayout);

        // Set the size hint for the item
        item->setSizeHint(itemWidget->sizeHint());

        // Store the video URL in the item
        item->setData(Qt::UserRole, *videoInfo.url);

        // Add the item to the ListWidget
        player_ui->listWidget->addItem(item);

        // Set the custom QWidget as the item widget in the ListWidget
        player_ui->listWidget->setItemWidget(item, itemWidget);
    }

    // If videos were found, start playing the first video in the playlist
    if (!videoFiles.isEmpty()) {
        togglePlayPause();

        // Update the selection of the first video item in the ListWidget
        QListWidgetItem *firstItem = player_ui->listWidget->item(currentVideoIndex);
        firstItem->setSelected(true);  // Set the first item as selected
    }
}

// Function to update the width of a child widget based on a percentage of the parent widget's width
void Player::updateChildWidgetWidth(QWidget *parentWidget, QWidget *childWidget, double percentage)
{
    int parentWidth = parentWidget->width();  // Get the width of the parent widget
    int childWidth = parentWidth * percentage;  // Calculate the child widget's width as a percentage of the parent
    childWidget->setFixedWidth(childWidth);  // Set the width of the child widget
}

// Play button clicked: starts playing the video
void Player::on_playButton_clicked()
{
    player->play();
}

// Pause button clicked: pauses the video
void Player::on_pauseButton_clicked()
{
    player->pause();
}

// Next button clicked: plays the next video in the playlist
void Player::on_nextButton_clicked()
{
    playNextVideo();
}

// Previous button clicked: plays the previous video in the playlist
void Player::on_previousButton_clicked()
{
    playPreviousVideo();
}

// Timer update: updates the progress slider based on current playback position
void Player::onTimerOut()
{
    int position = player->position();
    int duration = player->duration();
    if (position >= 0 && duration > 0) {
        int sliderValue = static_cast<int>((position * maxValue) / duration);
        player_ui->progressSlider->setValue(sliderValue);
    }
}

// Fast forward button clicked: skips forward 5 seconds in the video
void Player::onFastForward()
{
    // Get the current playback position
    qint64 currentTime = player->position();

    // Fast forward 5 seconds (5000 ms)
    qint64 newTime = currentTime + 5000;

    // Ensure the new time does not exceed the video duration
    if (newTime < player->duration()) {
        player->setPosition(newTime);
    } else {
        player->setPosition(player->duration());  // Go to the end of the video if new time exceeds duration
    }
}

// Fast rewind button clicked: skips backward 5 seconds in the video
void Player::onFastRewind()
{
    // Get the current playback position
    qint64 currentTime = player->position();

    // Rewind 5 seconds (5000 ms)
    qint64 newTime = currentTime - 5000;

    // Ensure the new time does not go below 0
    if (newTime > 0) {
        player->setPosition(newTime);
    } else {
        player->setPosition(0);  // Go to the start of the video if new time is less than 0
    }
}

// Play the next video in the playlist
void Player::playNextVideo()
{
    // If current video is the last one, loop back to the first video
    if (currentVideoIndex == playerList->mediaCount() - 1) {
        currentVideoIndex = 0;  // Go to the first video
    } else {
        currentVideoIndex++;  // Move to the next video
    }

    playerList->setCurrentIndex(currentVideoIndex);
    player->play();
    initData();
    adjustPlayPause();


    // Update the selected item in the list widget
    player_ui->listWidget->clearSelection();
    QListWidgetItem *item = player_ui->listWidget->item(currentVideoIndex);
    item->setSelected(true);

    // Log the name of the currently playing video
    QString currentVideoName = playerList->currentMedia().QMediaContent::request().url().fileName();
    qDebug() << "Current video: " << currentVideoName;
}

// Play the previous video in the playlist
void Player::playPreviousVideo()
{
    // If current video is the first one, loop back to the last video
    if (currentVideoIndex == 0) {
        currentVideoIndex = playerList->mediaCount() - 1;  // Go to the last video
    } else {
        currentVideoIndex--;  // Move to the previous video
    }

    playerList->setCurrentIndex(currentVideoIndex);
    player->play();
    initData();
    adjustPlayPause();

    // Update the selected item in the list widget
    player_ui->listWidget->clearSelection();
    QListWidgetItem *item = player_ui->listWidget->item(currentVideoIndex);
    item->setSelected(true);

    // Log the name of the currently playing video
    QString currentVideoName = playerList->currentMedia().QMediaContent::request().url().fileName();
    qDebug() << "Current video: " << currentVideoName;

    qDebug() << "Current index: " << currentVideoIndex;
}

// Toggle play/pause state and update the button text/icon accordingly
void Player::togglePlayPause()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        player_ui->playPauseButton->setText(QChar(0xe633));  // Set pause icon
    } else {
        player->play();
        player_ui->playPauseButton->setText(QChar(0xe628));  // Set play icon
    }
}

void Player::adjustPlayPause()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player_ui->playPauseButton->setText(QChar(0xe628));  // Set play icon
    } else {
        player_ui->playPauseButton->setText(QChar(0xe633));  // Set pause icon
    }
}



// Set the volume of the player
void Player::setVolume(int volume)
{
    player->setVolume(volume);  // Set the volume level
}

// Update the current time and total duration display on the UI
void Player::updateTimeDisplay()
{
    int currentPosition = player->position();
    int totalDuration = player->duration();

    // Update the current time label
    QTime currentTime(0, 0);
    currentTime = currentTime.addMSecs(currentPosition);
    player_ui->currentTimeLabel->setText(currentTime.toString("mm:ss /"));

    // Update the total duration label
    QTime totalTime(0, 0);
    totalTime = totalTime.addMSecs(totalDuration);
    player_ui->durationLabel->setText(totalTime.toString("mm:ss"));
}

// Handle video item click in the list widget
void Player::onVideoItemClicked(QListWidgetItem *item)
{
    // Get the clicked item's index and update currentVideoIndex
    int index = player_ui->listWidget->row(item);
    currentVideoIndex = index;
    playerList->setCurrentIndex(currentVideoIndex);

    qDebug() << "Current index: " << currentVideoIndex;

    player->play();

    // Update the play/pause button icon to pause
    player_ui->playPauseButton->setIcon(uiTool.pauseIcon);

    // Update the selected item in the list widget
    player_ui->listWidget->clearSelection();
    item->setSelected(true);
}

// Handle progress slider click: jump to the new position
void Player::onProgressSliderClicked()
{
    player->setPosition(player_ui->progressSlider->value() * player->duration() / maxValue);
}

// Handle progress slider movement: update position without timer interference
void Player::onProgressSliderMoved()
{
    progressTimer->stop();
    player->setPosition(player_ui->progressSlider->value() * player->duration() / maxValue);
}

// When the progress slider is released, start the timer again
void Player::onProgressSliderReleased()
{
    progressTimer->start();
}

// Automatically play the next video when the current video ends
void Player::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        playNextVideo();  // Play the next video when the current one ends
    }

    adjustPlayPause();
}

// Volume slider changed: adjust the volume based on slider value
void Player::onVolumeSliderChanged()
{
    int volumeValue = player_ui->volumeSlider->value();

    if (volumeValue == 2) {
        volumeValue = 0;  // Special handling for volume slider behavior
    } else if (volumeValue == 1) {
        volumeValue--;
    }

    player->setVolume(volumeValue);  // Set the volume to the new value

    // Update the volume button icon depending on the volume level
    if (volumeValue == 0) {
        player_ui->volumeButton->setText(QChar(0xe652));  // Mute icon
    } else {
        player_ui->volumeButton->setText(QChar(0xe609));  // Normal volume icon
    }
}

bool volumeState = true;  // Keeps track of whether the volume is muted or not

// Handle volume button click: toggle mute/unmute
void Player::onVolumeButtonClicked()
{
    int defaultVolume = 7;  // Default volume level

    if (volumeState) {
        previousVolume = player->volume();
        player->setVolume(0);  // Mute the player
        player_ui->volumeSlider->setValue(0);  // Set slider to 0 (mute)
        player_ui->volumeButton->setText(QChar(0xe652));  // Mute icon
    } else {
        if (previousVolume == 0) {
            player->setVolume(defaultVolume);  // Set to default volume if previously muted
            player_ui->volumeSlider->setValue(defaultVolume);
        } else {
            player->setVolume(previousVolume);  // Restore previous volume
            player_ui->volumeSlider->setValue(previousVolume);
        }

        player_ui->volumeButton->setText(QChar(0xe609));  // Normal volume icon
    }

    volumeState = !volumeState;  // Toggle the mute state
}

void Player::onLikeButtonCliked() {
    // Get the current stylesheet of the like button
    QString currentStyle = player_ui->likeButton->styleSheet();

    // Check the current text color of the button; if it's lightgray, change it to red, otherwise set it to lightgray
    if (currentStyle.contains("color: lightgray")) {
        currentStyle.replace("color: lightgray", "color: red");
    } else {
        currentStyle.replace("color: red", "color: lightgray");
    }

    // Update the button's stylesheet with the new color
    player_ui->likeButton->setStyleSheet(currentStyle);
}

void Player::onStarButtonCliked() {
    // Get the current stylesheet of the star button
    QString currentStyle = player_ui->starButton->styleSheet();

    // Check the current text color of the button; if it's lightgray, change it to gold (#fdb702), otherwise set it to lightgray
    if (currentStyle.contains("color: lightgray")) {
        currentStyle.replace("color: lightgray", "color: #fdb702");
    } else {
        currentStyle.replace("color: #fdb702", "color: lightgray");
    }

    // Update the button's stylesheet with the new color
    player_ui->starButton->setStyleSheet(currentStyle);
}

void Player::displayComment(QLabel* commentLabel, const QString& commentText)
{
    if (!commentLabel) return;  // Ensure the provided QLabel is not null

    // Set the comment text on the label
    commentLabel->setText(commentText);
    commentLabel->setStyleSheet(
        "color: white; "                       // Text color (white)
        "font-size: 22px; "                    // Font size
        "font-family: 'Comic Sans MS'; "       // Font style (can use simpler fonts like Arial)
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "   stop:0 rgba(255, 105, 180, 200), "  // Gradient color (from pink to deep pink)
        "   stop:1 rgba(255, 182, 193, 180)); " // End color (light pink)
        "border: 2px solid rgba(255, 105, 180, 200); "  // Border color matching the gradient
        "padding: 10px 15px; "                 // Padding (10px top-bottom, 15px left-right)
        "opacity: 0.9; "                       // Set opacity for a softer background color
        "text-align: left;"                    // Text alignment (can be changed to center if needed)
        );

    commentLabel->raise();  // Bring the label to the front
    commentLabel->setAlignment(Qt::AlignCenter);

    commentLabel->adjustSize();  // Adjust size to fit content

    // Ensure label does not exceed the width of the video widget
    int maxWidth = player_ui->videoWidget->width();
    int labelWidth = commentLabel->width();
    if (labelWidth > maxWidth) {
        commentLabel->setFixedWidth(maxWidth);  // Set max width for the label
    }

    // Generate a random Y position within the top portion of the video widget
    int randomY = qrand() % (player_ui->videoWidget->height() / 3);  // Random Y position within top 1/3

    // Set the initial position of the comment label: outside the right edge of the video widget
    commentLabel->move(player_ui->videoWidget->width(), randomY);

    // Use QPropertyAnimation to animate the label from right to left
    animateComment(commentLabel);

    // Clear the comment input area
    player_ui->commentArea->clear();
}

void Player::onShareButtonClicked()
{
    // Show a message box indicating that the share was successful
    const int result = QMessageBox::information(
        NULL,
        QString("Tomeo"),
        QString("Share Successful!"));
}

void Player::onSendButtonClicked()
{
    // Get the comment text from the input area
    QString commentText = player_ui->commentArea->toPlainText();
    if (commentText.isEmpty()) {
        return;  // Do nothing if the comment is empty
    }

    // Add the comment to the comment list
    addComment("You", commentText, ":/avatar6.png", getCurrentTime());

    // Set the scroll bar value to the maximum, so it scrolls to the bottom
    QScrollBar *scrollBar = player_ui->commentList->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());

    bool foundEmptyLabel = false;
    QLabel* availableLabel = nullptr;

    // Find an available QLabel to display the comment
    if (!foundEmptyLabel && player_ui->commentLabel1->text().isEmpty()) {
        availableLabel = player_ui->commentLabel1;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel2->text().isEmpty()) {
        availableLabel = player_ui->commentLabel2;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel3->text().isEmpty()) {
        availableLabel = player_ui->commentLabel3;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel4->text().isEmpty()) {
        availableLabel = player_ui->commentLabel4;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel5->text().isEmpty()) {
        availableLabel = player_ui->commentLabel5;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel6->text().isEmpty()) {
        availableLabel = player_ui->commentLabel6;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel7->text().isEmpty()) {
        availableLabel = player_ui->commentLabel7;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel8->text().isEmpty()) {
        availableLabel = player_ui->commentLabel8;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel9->text().isEmpty()) {
        availableLabel = player_ui->commentLabel9;
        foundEmptyLabel = true;
    } else if (!foundEmptyLabel && player_ui->commentLabel10->text().isEmpty()) {
        availableLabel = player_ui->commentLabel10;
        foundEmptyLabel = true;
    }

    // If an available label is found, display the comment
    if (availableLabel) {
        displayComment(availableLabel, commentText);
    }
}

void Player::animateComment(QLabel *commentLabel)
{
    // Create an animation for the comment label
    QPropertyAnimation *animation = new QPropertyAnimation(commentLabel, "pos");
    animation->setDuration(12000);  // Set the animation duration to 12 seconds
    animation->setStartValue(QPoint(player_ui->videoWidget->width(), commentLabel->y()));  // Start at the right edge with a random Y position
    animation->setEndValue(QPoint(-commentLabel->width(), commentLabel->y()));  // End at the left edge with the same Y position

    // Set the easing curve for the animation to make it non-linear
    animation->setEasingCurve(QEasingCurve::Linear);

    // Connect the animation finished signal to clear the label's text after the animation is complete
    connect(animation, &QPropertyAnimation::finished, this, [commentLabel]() {
        commentLabel->clear();  // Clear the comment text after the animation finishes
    });

    // Start the animation and delete it when finished
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Player::updateVideoTitle()
{
    int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");  // Load the font file
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);  // Get the font family name
    QFont watchIconFont = QFont(fontName);  // Create a font object for action icons
    watchIconFont.setPixelSize(30);

    player_ui->watchIcon->setStyleSheet(
        "QLabel {"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "}");

    player_ui->watchLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Comic Sans MS';"
        "   color: white;"                       // Set the text color of the button to white
        "   background-color: transparent;"      // Set the background color to transparent
        "   border: none;"                       // No border around the button
        "   font-size: 30px;"
        "}");

    // Get the current video filename from the media player
    QString currentVideoName = playerList->currentMedia().QMediaContent::request().url().fileName();
    player_ui->videoLabel->setText("Currently Playing: " + currentVideoName);  // Display the video name in the UI

    int randomViewers = QRandomGenerator::global()->bounded(50, 1000);  // 生成50到1000之间的随机数
    player_ui->watchIcon->setFont(watchIconFont);
    player_ui->watchIcon->setText(QChar(0xe61d));
    player_ui->watchLabel->setText(QString::number(randomViewers) + " people are watching");

    qDebug() << currentVideoName;  // Log the video name for debugging purposes
}

void Player::addComment(const QString &username, const QString &commentText, const QString &avatarPath, const QString &timestamp)
{
    CommentData data = {username, commentText, avatarPath, timestamp};  // Create a CommentData structure

    // Create a new QListWidgetItem to represent the comment
    QListWidgetItem *item = new QListWidgetItem();

    // Create a custom widget to display the comment
    QWidget *commentWidget = createCommentWidget(data);

    // Set the size hint for the item, so it matches the width of the comment list widget
    item->setSizeHint(QSize(player_ui->commentList->width() - 20, commentWidget->sizeHint().height()));

    // Add the item and the widget to the comment list
    player_ui->commentList->addItem(item);
    player_ui->commentList->setItemWidget(item, commentWidget);
}

QWidget* Player::createCommentWidget(const CommentData &data) {
    // Create the outer widget to contain the comment
    QWidget *commentWidget = new QWidget();
    commentWidget->setStyleSheet("background-color: #f0f0f0; border-radius: 10px; padding: 10px;");

    // Create a vertical layout for the comment
    QVBoxLayout *mainLayout = new QVBoxLayout(commentWidget);

    // ====================== Avatar and Username Section ======================
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);  // Set the spacing between avatar and username

    // Avatar image
    QLabel *avatarLabel = new QLabel(commentWidget);
    avatarLabel->setPixmap(QPixmap(data.avatarPath).scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    avatarLabel->setFixedSize(80, 80);
    avatarLabel->setStyleSheet("border-radius: 5px;");  // Make the avatar round

    // Username label
    QLabel *usernameLabel = new QLabel(data.username, commentWidget);
    usernameLabel->setObjectName(QString("usernameLabel_%1").arg(data.username));
    usernameLabel->setStyleSheet(
        "font-family: 'Comic Sans MS';"
        "font-weight: bold;"
        "font-style: italic;"
        "color: #333333;"
        "font-size: 36px;"
        "over-flow: "
        );

    // Add avatar and username to the header layout
    headerLayout->addWidget(avatarLabel);
    headerLayout->addWidget(usernameLabel);
    headerLayout->addStretch();  // Add stretch space to align the avatar and username to the left

    // ====================== Comment Text Section ======================
    QLabel *commentTextLabel = new QLabel(data.commentText, commentWidget);
    commentTextLabel->setWordWrap(true);  // Allow the text to wrap
    commentTextLabel->setStyleSheet(
        "font-family: 'Comic Sans MS';"
        "color: #555555;"
        "font-size: 24px;"
        );
    commentTextLabel->setMinimumHeight(120);  // Set the minimum height for the comment text

    // ====================== Timestamp Section ======================
    QLabel *timestampLabel = new QLabel(data.timestamp, commentWidget);
    timestampLabel->setStyleSheet(
        "font-family: 'Comic Sans MS';"
        "color: #999999;"
        "font-size: 16px;"
        );

    // ====================== Add all sections to the main layout ======================
    mainLayout->addLayout(headerLayout);     // Add avatar and username section
    mainLayout->addWidget(commentTextLabel); // Add comment text section
    mainLayout->addWidget(timestampLabel);   // Add timestamp section

    return commentWidget;  // Return the completed widget
}

void Player::initComments() {
    // Initialize the comments by adding sample data
    addComment("Alice", "This video is awesome! 😍", ":/avatar1.png", "2024-06-01 10:30");
    addComment("Bob", "I totally agree with you!", ":/avatar2.png", "2024-06-01 11:00");
    addComment("Charlie", "This part made me laugh 😂", ":/avatar3.png", "2024-06-01 11:15");
    addComment("David", "Amazing content, keep it up!", ":/avatar4.png", "2024-06-01 11:45");
    addComment("Eve", "Can't stop watching this! 🤩", ":/avatar5.png", "2024-06-01 12:00");
}

void Player::onSpeedButtonClicked()
{
    // Toggle playback speed based on the current rate
    if (currentPlaybackRate == 1.0) {
        currentPlaybackRate = 1.5;  // Set playback rate to 1.5x
        player_ui->speedButton->setText("1.5x");
    } else if (currentPlaybackRate == 1.5) {
        currentPlaybackRate = 2.0;  // Set playback rate to 2.0x
        player_ui->speedButton->setText("2.0x");
    } else {
        currentPlaybackRate = 1.0;  // Reset to 1.0x speed
        player_ui->speedButton->setText("1.0x");
    }

    // Set the playback speed for the media player
    player->setPlaybackRate(currentPlaybackRate);
}

QString Player::getCurrentTime() {
    // Get the current system time
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Format the time as a string
    QString formattedTime = currentDateTime.toString("yyyy-MM-dd HH:mm");

    return formattedTime;  // Return the formatted current time
}

void Player::initData()
{
    double randomLike = QRandomGenerator::global()->generateDouble() * (100.0 - 50.0);
    randomLike = floor(randomLike * 10 + 0.5) / 10.0;  // 保留一位小数

    double randomStar = QRandomGenerator::global()->generateDouble() * (30.0 - 5.0);;
    randomStar = floor(randomStar * 10) / 10.0;

    double randomComment = QRandomGenerator::global()->generateDouble() * (10.0 - 5.0);;
    randomComment = floor(randomComment * 10) / 10.0;

    player_ui->likeCount->setText(QString::number(randomLike, 'f', 1) + "k");

    player_ui->starCount->setText(QString::number(randomStar, 'f', 1) + "k");

    player_ui->commentCount->setText(QString::number(randomComment, 'f', 1) + "k");
}
