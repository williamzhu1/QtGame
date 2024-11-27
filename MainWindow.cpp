#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    viewTabs(new QTabWidget(this)),
    graphicsTab(new QWidget(this)),
    textualTab(new QWidget(this)),
    graphicsMessageWidget(new QTextEdit(this)),
    textualMessageWidget(new QTextEdit(this)),
    startButton(new QPushButton("Start", this)),
    pauseButton(new QPushButton("Pause", this)),
    quitButton(new QPushButton("Quit", this)),
    autoPlayButton(new QPushButton("Auto Play", this)),
    animationDelaySlider(new QSlider(Qt::Horizontal, this)),
    heuristicWeightFactorSlider(new QSlider(Qt::Horizontal, this)),
    heuristicWeightFactorLabel(new QLabel("Heuristic weight factor", this)),
    animationDelayLabel(new QLabel("Animation delay", this)),
    energyLabel(new QLabel("Energy: ", this)),
    energyValueLabel(new QLabel("100.00", this)),
    playerNumberComboBox(new QComboBox(this)),
    playerNumberLabel(new QLabel("Number of Players", this)),
    difficultyLevelComboBox(new QComboBox(this)),
    difficultyLevelLabel(new QLabel("Difficulty Level", this)),
    isGamePaused(false),
    gameController(new GameController(this))
{
    setCentralWidget(centralWidget);
    setupUI();

    pauseButton->setEnabled(false);
    pauseButton->setStyleSheet("background-color: grey;");
    autoPlayButton->setEnabled(false);
    autoPlayButton->setStyleSheet("background-color: grey;");
    quitButton->setEnabled(false);
    quitButton->setStyleSheet("background-color: grey;");

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);
    connect(autoPlayButton, &QPushButton::clicked, this, &MainWindow::onAutoPlayButtonClicked);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::onQuitButtonClicked);
    connect(viewTabs, &QTabWidget::currentChanged, this, &MainWindow::onViewTabChanged);
    connect(heuristicWeightFactorSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);

    connect(gameController, &GameController::viewUpdateRequested, this, &MainWindow::onViewUpdateRequested);
    connect(gameController, &GameController::sendTextToGUI, this, &MainWindow::onDisplayText);
    connect(gameController, &GameController::updateStatusDisplay, this, &MainWindow::onUpdateStatusDisplay);
    connect(gameController, &GameController::sendGameWon, this, &MainWindow::onGameWon);
    connect(gameController, &GameController::sendGameLost, this, &MainWindow::onGameLost);
    connect(gameController, &GameController::autoPlayed, this, &MainWindow::onAutoPlayed);
}


MainWindow::~MainWindow() {
    delete gameController;
}


void MainWindow::setupUI()
{
    // Set the window title
    setWindowTitle("MainWindow");

    // Initialize the player number combo box
    playerNumberComboBox->addItem("1");
    playerNumberComboBox->addItem("2");

    // Initialize the difficulty level combo box
    difficultyLevelComboBox->addItem("Easy");
    difficultyLevelComboBox->addItem("Medium");
    difficultyLevelComboBox->addItem("Hard");

    QVBoxLayout *graphicsLayout = new QVBoxLayout();
    QVBoxLayout *textualLayout = new QVBoxLayout();

    // Add a widget for game messages in the graphics tab
    graphicsMessageWidget->setReadOnly(true);
    graphicsMessageWidget->setStyleSheet("background-color: white;");
    graphicsMessageWidget->setFixedHeight(100);

    viewTabs->addTab(graphicsTab, "Graphics");

    graphicsLayout->addWidget(graphicsMessageWidget);
    graphicsTab->setLayout(graphicsLayout);

    // Add a widget for game messages in the textual tab
    textualMessageWidget->setReadOnly(false);
    textualMessageWidget->setStyleSheet("background-color: white;");
    textualMessageWidget->setFixedHeight(100);
    textualMessageWidget->setPlaceholderText("> Type command here and press Enter...");
    textualMessageWidget->installEventFilter(this);

    heuristicWeightFactorSlider->setRange(100, 200);
    heuristicWeightFactorSlider->setValue(100);


    viewTabs->addTab(textualTab, "Textual");

    textualLayout->addWidget(textualMessageWidget);
    textualTab->setLayout(textualLayout);

    // Set up the controls
    startButton->setFixedSize(100, 30);
    quitButton->setFixedSize(100, 30);
    pauseButton->setFixedSize(100, 30);
    autoPlayButton->setFixedSize(100, 30);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(viewTabs);

    // Control layout
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(autoPlayButton);
    controlLayout->addWidget(quitButton);
    
    controlLayout->addWidget(playerNumberLabel);
    controlLayout->addWidget(playerNumberComboBox);
    controlLayout->addWidget(difficultyLevelLabel);
    controlLayout->addWidget(difficultyLevelComboBox);
    controlLayout->addWidget(animationDelayLabel);
    controlLayout->addWidget(animationDelaySlider);
    controlLayout->addWidget(heuristicWeightFactorLabel);
    controlLayout->addWidget(heuristicWeightFactorSlider);

    controlLayout->addStretch(1); // Push the remaining elements to the right

    const int maxHealth = 5;
    const QSize heartSize(30, 30);

    for (int i = 0; i < maxHealth; ++i) {
        QLabel* healthLabel = new QLabel(this);
        QPixmap heartPixmap(":/images/healthpack/Idle/2.png");
        healthLabel->setPixmap(heartPixmap.scaled(heartSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        healthLabels.append(healthLabel);
        controlLayout->addWidget(healthLabel);
    }

    controlLayout->addWidget(energyLabel);
    controlLayout->addWidget(energyValueLabel);

    // Add control layout to the main layout
    mainLayout->addLayout(controlLayout);

    // Set the central widget layout
    centralWidget->setLayout(mainLayout);
}

void MainWindow::onStartButtonClicked()
{
    QString numberOfPlayers = playerNumberComboBox->currentText();
    QString difficultyLevel = difficultyLevelComboBox->currentText();

    QString message = QString("Game started with %1 players and level of difficulty: %2")
                        .arg(numberOfPlayers, difficultyLevel);

    graphicsMessageWidget->append(message);
    textualMessageWidget->append(message);

    startButton->setEnabled(false);
    startButton->setStyleSheet("background-color: grey;");

    pauseButton->setEnabled(true);
    pauseButton->setStyleSheet("");

    autoPlayButton->setEnabled(true);
    autoPlayButton->setStyleSheet("");

    quitButton->setEnabled(true);
    quitButton->setStyleSheet("");

    playerNumberComboBox->setEnabled(false);
    playerNumberComboBox->setStyleSheet("background-color: grey;");

    difficultyLevelComboBox->setEnabled(false);
    difficultyLevelComboBox->setStyleSheet("background-color: grey;");

    // Interact with game controller
    gameController->readGameStarted(true);
    gameController->readGameNumberOfPlayers(numberOfPlayers);
    gameController->readGameDifficultyLevel(difficultyLevel);
    if (startButton->text() != "Restart") {
        gameController->initializeWorld();
    } else {
        gameController->reInitializeWorld();
    }
}

void MainWindow::onPauseButtonClicked()
{
    if (isGamePaused) {
        graphicsMessageWidget->append("Game unpaused!");
        textualMessageWidget->append("Game unpaused!");
        gameController->readGameStarted(true);
        pauseButton->setText("Pause");
        autoPlayButton->setEnabled(true);
        autoPlayButton->setStyleSheet("");
        isGamePaused = false;
    } else {
        graphicsMessageWidget->append("Game paused!");
        textualMessageWidget->append("Game paused!");
        gameController->readGameStarted(false);
        pauseButton->setText("Unpause");
        autoPlayButton->setEnabled(false);
        autoPlayButton->setStyleSheet("background-color: grey;");
        isGamePaused = true;
    }
}

void MainWindow::onAutoPlayButtonClicked()
{
    graphicsMessageWidget->append("Auto playing...");
    textualMessageWidget->append("Auto playing...");
    autoPlayButton->setEnabled(false);
    pauseButton->setEnabled(false);
    gameController->readGameAutoplayed();
}

void MainWindow::onQuitButtonClicked()
{
    graphicsMessageWidget->append("Game exited!");
    textualMessageWidget->append("Game exited!");

    startButton->setEnabled(true);
    startButton->setStyleSheet("");

    pauseButton->setText("Pause");
    pauseButton->setEnabled(false);
    pauseButton->setStyleSheet("background-color: grey;");
    isGamePaused = false;

    autoPlayButton->setEnabled(false);
    autoPlayButton->setStyleSheet("background-color: grey;");

    quitButton->setEnabled(false);
    quitButton->setStyleSheet("background-color: grey;");
    gameController->readGameStarted(false);

    playerNumberComboBox->setEnabled(true);
    playerNumberComboBox->setStyleSheet("");
    gameController->readGameNumberOfPlayers("0");

    difficultyLevelComboBox->setEnabled(true);
    difficultyLevelComboBox->setStyleSheet("");
    gameController->readGameDifficultyLevel("Not Selected");

    gameController->setGameOver();
    startButton->setText("Restart");
}

void MainWindow::onViewTabChanged(int index)
{
    if (index == 0) {
        gameController->switchTo2DView();
    } else if (index == 1) {
        textualMessageWidget->clear();
        gameController->switchToTextView();
        textualMessageWidget->setFocus();

    }
}

void MainWindow::onViewUpdateRequested(QWidget* view) {
    displayView(view);
}

void MainWindow::displayView(QWidget* view) {
    if (!viewTabs || viewTabs->count() == 0) {
        return;
    }

    QWidget* currentTab = viewTabs->currentWidget();

    if (currentTab) {
        QLayout* layout = currentTab->layout();
        layout->addWidget(view);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!gameController->isGameOver()) {
        switch (event->key()) {
        case Qt::Key_W: gameController->onUpArrowPressed(); break;
        case Qt::Key_S: gameController->onDownArrowPressed(); break;
        case Qt::Key_A: gameController->onLeftArrowPressed(); break;
        case Qt::Key_D: gameController->onRightArrowPressed(); break;
        default: QMainWindow::keyPressEvent(event); break;
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onDisplayText(const QString& text) {
    graphicsMessageWidget->append(text);

    // Append the text to the textualMessageWidget instead of setting plain text
    textualMessageWidget->moveCursor(QTextCursor::End);
    textualMessageWidget->append(text);
    textualMessageWidget->moveCursor(QTextCursor::End);
}


void MainWindow::onGameWon(){
    autoPlayButton->setEnabled(false);
    QMessageBox::information(this, "Game Over", "YOU WIN!");
    // Disconnect the game won and lost signals from their slots
    disconnect(gameController, &GameController::sendGameWon, this, &MainWindow::onGameWon);
    disconnect(gameController, &GameController::sendGameLost, this, &MainWindow::onGameLost);

    // Now, the onGameWon and onGameLost functions will not be called when sendGameWon and sendGameLost signals are emitted.

}

void MainWindow::onGameLost(){
    autoPlayButton->setEnabled(false);
    QMessageBox::information(this, "Game Over", "DIE! YOU'VE GOT NOTHING! YOU LOSE!");
    // Disconnect the game won and lost signals from their slots
    disconnect(gameController, &GameController::sendGameWon, this, &MainWindow::onGameWon);
    disconnect(gameController, &GameController::sendGameLost, this, &MainWindow::onGameLost);

    // Now, the onGameWon and onGameLost functions will not be called when sendGameWon and sendGameLost signals are emitted.

}

void MainWindow::onAutoPlayed(){
    if (!gameController->isGameOver()){
        autoPlayButton->setEnabled(true);
        pauseButton->setEnabled(true);
    }
    return;
}

void MainWindow::onUpdateStatusDisplay(int currentHealth, float currentEnergy){

    for (int i = 0; i < healthLabels.size(); ++i) {
        if (i < currentHealth) {
            healthLabels[i]->setVisible(true);
        } else {
            healthLabels[i]->setVisible(false);
        }
    }
    QString b;
    b.setNum(currentEnergy);
    energyValueLabel->setText(b);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == textualMessageWidget && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            // Handle the Enter key press
            QString command = textualMessageWidget->toPlainText().trimmed();
            if (command.isEmpty()){
                return QMainWindow::eventFilter(watched, event);
            }
            gameController->processCommand(command);
            textualMessageWidget->clear();
            return true; // indicate that the event was handled
        }
    }
    // Call base class method for default processing
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onSliderValueChanged(int value) {
    // Emit the custom signal sliderValueChanged when the slider value changes
    gameController->getWorldController()->setHeuristicWeight(static_cast<float>(value) / 100.0);
}
