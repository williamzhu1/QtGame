#include "Controller/ViewController.h"

ViewController::ViewController(QObject *parent) :
    QObject(parent),
    worldController(WorldController::getInstance()),
    currentView(nullptr) {
    
}


void ViewController::initializeViews() {
    game2DView = std::make_shared<Game2DView>(nullptr);
    gameTextView = std::make_shared<GameTextView>(nullptr);

    const WorldModel& world = worldController.getCurrentWorld();
    game2DView->setCurrentWorld(world);
    gameTextView->setCurrentWorld(world);

    // Initialize the views
    game2DView->initializeView();
    gameTextView->initializeView();

    // Optionally set the initial view
    currentView = game2DView.get();
    //currentView = game2DView;
    emit viewUpdated(currentView);

    connect(&worldController, &WorldController::updateprotagonistPosition, this, &ViewController::onUpdateProtagonistPosition);
    connect(&worldController, &WorldController::updateLevel, this, &ViewController::updateLevel);
}

void ViewController::switchTo2DView() {
    if (currentView != game2DView.get()) {
        currentView = game2DView.get();
        emit viewUpdated(currentView);
    }
}

void ViewController::switchToTextView() {
    if (currentView != gameTextView.get()) {
        currentView = gameTextView.get();
        emit viewUpdated(currentView);
    }
}

QWidget* ViewController::getCurrentView() const {
    return currentView;
}

void ViewController::onUpdateProtagonistPosition(int protagonistIndex) {
    Q_UNUSED(protagonistIndex);
    game2DView->updateView();
    gameTextView->updateView();
}

void ViewController::updateLevel() {
    const WorldModel& world = worldController.getCurrentWorld();
    game2DView->setCurrentWorld(world);
    gameTextView->setCurrentWorld(world);
    game2DView->initializeView();
    gameTextView->initializeView();
    game2DView->updateView();
    gameTextView->updateView();
}
