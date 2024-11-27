#include "GameController.h"

GameController::GameController(QObject *parent)
    : QObject(parent),
    worldController(WorldController::getInstance()),
    viewController(ViewController::getInstance()),
    isGameStarted(false),
    isGamePaused(false)
{
    // Connect signals and slots
    connect(&viewController, &ViewController::viewUpdated, this, &GameController::onViewUpdated);
    connect(&worldController, &WorldController::gameWon, this, &GameController::setWon);
    connect(&worldController, &WorldController::onProtagonistDead, this, &GameController::setLost);
    connect(&worldController, &WorldController::onHealthAndEnergyUpdate, this, &GameController::onUpdateHealthAndEnergy);


    commandMap["up"] = [this](const QStringList& args) { Q_UNUSED(args); worldController.moveProtagonist(UP); };
    commandMap["down"] = [this](const QStringList& args) { Q_UNUSED(args); worldController.moveProtagonist(DOWN); };
    commandMap["left"] = [this](const QStringList& args) { Q_UNUSED(args); worldController.moveProtagonist(LEFT); };
    commandMap["right"] = [this](const QStringList& args) { Q_UNUSED(args); worldController.moveProtagonist(RIGHT); };
    commandMap["goto"] = [this](const QStringList& args) {
        if(args.size() >= 3) {
            int x = args[1].toInt();
            int y = args[2].toInt();
            coordinate coord(x,y);
            worldController.moveProtagonist(coord);
        }
    };

    commandMap["attack"] = [this](const QStringList& args){
        Q_UNUSED(args);
        const WorldModel& currentWorld = worldController.getCurrentWorld();
        const coordinate& protagonistPos = currentWorld.getProtagonists()[0]->getPosition();
        coordinate coord1 = worldController.getCurrentWorld().findNearestEnemy();
        coordinate coord2 = worldController.getCurrentWorld().findNearestPEnemy();
        coordinate coord3 = worldController.getCurrentWorld().findNearestXEnemy();
        // Calculate distances to each enemy
        double dist1 = protagonistPos.distanceTo(coord1);
        double dist2 = protagonistPos.distanceTo(coord2);
        double dist3 = protagonistPos.distanceTo(coord3);

        // Determine the nearest enemy
        coordinate nearestCoord = coord1; // Assume the regular enemy is the closest
        double minDist = dist1;

        if (dist2 < minDist) {
            nearestCoord = coord2;
            minDist = dist2;
        }
        if (dist3 < minDist) {
            nearestCoord = coord3;
        }

        // Move the protagonist to the nearest enemy
        worldController.moveProtagonist(nearestCoord);
    };
    commandMap["take"] = [this](const QStringList& args){
        Q_UNUSED(args);
        coordinate coord = worldController.getCurrentWorld().findNearestHealthPack();
        worldController.moveProtagonist(coord);
    };
    commandMap["help"] = [this](const QStringList& args) {
        Q_UNUSED(args); // This macro indicates that the args parameter is intentionally unused
        this->displayHelp();
    };
}

GameController::~GameController() {
    
}

void GameController::readGameStarted(bool isStarted) {
    isGameStarted = isStarted;
}

void GameController::readGameAutoplayed() {
    worldController.autoplay();
    emit autoPlayed();
}

void GameController::readGameNumberOfPlayers(const QString &numberOfPlayers) {
    gameNumberOfPlayers = numberOfPlayers;
}

void GameController::readGameDifficultyLevel(const QString &difficultyLevel) {
    gameDifficultyLevel = difficultyLevel;
}

void GameController::decideGameParameters() {
    if (gameDifficultyLevel == "Easy") {
        gameDifficultyIdx = 1;
        gamePRatio = (gameNumberOfPlayers.toInt() == 1) ? 0.1 : 0.2;
    } else if (gameDifficultyLevel == "Medium") {
        gameDifficultyIdx = 2;
        gamePRatio = (gameNumberOfPlayers.toInt() == 1) ? 0.2 : 0.4;
    } else if (gameDifficultyLevel == "Hard") {
        gameDifficultyIdx = 3;
        gamePRatio = (gameNumberOfPlayers.toInt() == 1) ? 0.3 : 0.6;
    }
}

void GameController::initializeWorld() {
    decideGameParameters();

    worldController.createWorld(":/images/world_images/worldmap.png", gameNumberOfPlayers.toInt(), gameDifficultyIdx, gamePRatio);
    worldController.createWorld(":/images/world_images/worldmap2.png", gameNumberOfPlayers.toInt(), gameDifficultyIdx, gamePRatio);
    worldController.createWorld(":/images/world_images/worldmap3.png", gameNumberOfPlayers.toInt(), gameDifficultyIdx, gamePRatio);
    worldController.createWorld(":/images/world_images/worldmap4.png", gameNumberOfPlayers.toInt(), gameDifficultyIdx, gamePRatio);
    worldController.createWorld(":/images/world_images/worldmap5.png", gameNumberOfPlayers.toInt(), gameDifficultyIdx, gamePRatio);
    worldController.getCurrentWorld().getProtagonists()[0]->setHealth(5);
    worldController.getCurrentWorld().getProtagonists()[0]->setEnergy(100);

    viewController.initializeViews();
}

void GameController::reInitializeWorld() {
    worldController.getCurrentWorld().getProtagonists()[0]->setHealth(5);
    worldController.getCurrentWorld().getProtagonists()[0]->setEnergy(100);
}

void GameController::switchTo2DView() {
    viewController.switchTo2DView();
}

void GameController::switchToTextView() {
    viewController.switchToTextView();
}

void GameController::onViewUpdated(QWidget* currentView) {
    emit viewUpdateRequested(currentView);
}

void GameController::onUpArrowPressed() {
    if (isGameStarted) {
        worldController.moveProtagonist(UP);
    }
}

void GameController::onDownArrowPressed() {
    if (isGameStarted) {
        worldController.moveProtagonist(DOWN);
    }
}

void GameController::onLeftArrowPressed() {
    if (isGameStarted) {
        worldController.moveProtagonist(LEFT);
    }
}

void GameController::onRightArrowPressed() {
    if (isGameStarted) {
        worldController.moveProtagonist(RIGHT);
    }
}

void GameController::onUpdateHealthAndEnergy() {
    int gameHealth = worldController.getCurrentWorld().getProtagonists()[0]->getHealth();
    float gameEnergy = worldController.getCurrentWorld().getProtagonists()[0]->getEnergy();
    emit updateStatusDisplay(gameHealth, gameEnergy);
}

void GameController::setWon() {
    this->setGameOver();
    emit sendGameWon();
}

void GameController::setLost() {
    this->setGameOver();
    emit sendGameLost();
}

bool GameController::isGameOver() {
    if (!isGameStarted) {
        return true;
    }
    else {
        return false;
    }
}

void GameController::setGameOver() {
    isGameStarted = false;
}

void GameController::processCommand(const QString& command) {
    QStringList args = command.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QString action = args.first().toLower();

    auto cmdIter = commandMap.find(action);
    if (cmdIter != commandMap.end()) {
        cmdIter->second(args); // Execute the command
    } else {
        qDebug() << "Unknown command";
        displayHelp();
    }
}


void GameController::displayHelp() const {
    QString helpText =
        "Available commands:\n"
        "  - up: Move the protagonist up\n"
        "  - down: Move the protagonist down\n"
        "  - left: Move the protagonist left\n"
        "  - right: Move the protagonist right\n"
        "  - goto x y: Move to the specified coordinates (x, y)\n"
        "  - attack: Attack the nearest enemy\n"
        "  - take: Take the nearest health pack\n"
        "  - help: Display this help text\n";

    qDebug() << helpText;
    emit sendTextToGUI(helpText);
}

WorldController* GameController::getWorldController(){
    return &worldController;
}

