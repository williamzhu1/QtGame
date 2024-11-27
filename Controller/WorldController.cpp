#include "Controller/WorldController.h"

WorldController::WorldController(){
    // Basic initializations, if any
}
void WorldController::createWorld(QString map, int gameNumberOfPlayers, int gameDifficultyIdx, float pRatio) {
    Q_UNUSED(gameNumberOfPlayers);
    /**
     * given gameDifficultyIdx assign the number of healthpacks
    */
    int nrOfHealthpacks = 0;
    int nrOfEnemies = 0;
    switch (gameDifficultyIdx)
    {
    case 1:
        nrOfHealthpacks = 5;
        nrOfEnemies = 8;
        break;
    case 2:
        nrOfHealthpacks = 3;
        nrOfEnemies = 15;
        break;
    case 3:
        nrOfHealthpacks = 1;
        nrOfEnemies = 30;
        break;
    default:
        break;
    }
    difficultyIdx = gameDifficultyIdx;
    if(firstWorld){
        worlds.push_back(std::make_unique<WorldModel>(map, nrOfEnemies, nrOfHealthpacks, pRatio, true));
        firstWorld = false;
        currentWorldIndex = 0;
        currentWorld = worlds[currentWorldIndex].get();
    }
    else{
        worlds.push_back(std::make_unique<WorldModel>(map, nrOfEnemies+3, nrOfHealthpacks, pRatio, false));
    }

    for(const auto& protagonist: currentWorld->getProtagonists()){
        connect(protagonist.get(), &ProtagonistModel::protagonistDead, this, &WorldController::onProtagonistDead);
        connect(protagonist.get(), &ProtagonistModel::healthUpdate, this, &WorldController::onHealthAndEnergyUpdate);
        connect(protagonist.get(), &ProtagonistModel::energyUpdate, this, &WorldController::onHealthAndEnergyUpdate);
    }
    qDebug() << "Nearest Healthpack: " << currentWorld->findNearestHealthPack().getXPos() << " " << currentWorld->findNearestHealthPack().getYPos();
    qDebug() << "Nearest Enemy: " << currentWorld->findNearestEnemy().getXPos() << " " << currentWorld->findNearestEnemy().getYPos();
    qDebug() << "Nearest PEnemy: " << currentWorld->findNearestPEnemy().getXPos() << " " << currentWorld->findNearestPEnemy().getYPos();
    qDebug() << "Nearest XEnemy: " << currentWorld->findNearestXEnemy().getXPos() << " " << currentWorld->findNearestXEnemy().getYPos();
}

int WorldController::getRows() const
{
    return currentWorld->getRows();
}

int WorldController::getCols() const
{
    return currentWorld->getCols();
}

int WorldController::getNumOfProtagonists() const
{
    return currentWorld->getProtagonists().size();
}

int WorldController::getDifficultyIdx() const
{
    return difficultyIdx;
}

void WorldController::deleteEnemy(coordinate coord)
{
    currentWorld->deleteEnemy(coord);
}

void WorldController::deletePsnTile(coordinate coord)
{
    currentWorld->deletePsnTile(coord);
}

void WorldController::onEncounterEnemy() {
    qDebug() << "Encountered an enemy!" << "\n";
    if (currentWorld->getProtagonists()[0]->getHealth() > 0) {
        currentWorld->getProtagonists()[0]->attack();
        currentWorld->currentEnemy->attack();
    }
    else {
        currentWorld->getProtagonists()[0]->setHealth(0);
        qDebug() << "You died!" << "\n";
    }
}

void WorldController::onEncounterHealthPack() {
    qDebug() << "Encountered a health pack!" << "\n";
    if (currentWorld->getProtagonists()[0]->getHealth() < 5) {
        currentWorld->getProtagonists()[0]->setHealth(currentWorld->getProtagonists()[0]->getHealth() + 1);
        currentWorld->removeHealthpack(currentWorld->getProtagonists()[0]->getPosition());
    }
    else {
        qDebug() << "Health is full!" << "\n";
    }
}

void WorldController::onEncounterPEnemy() {
    qDebug() << "Encountered an penemy!" << "\n";
    if (currentWorld->getProtagonists()[0]->getHealth() > 0) {
        currentWorld->getProtagonists()[0]->attack();
        currentWorld->currentPEnemy->attack();
        currentWorld->setAffectedTiles(false, currentWorld->currentPEnemy->getPoisonLevel());
    }
    else {
        currentWorld->getProtagonists()[0]->setHealth(0);
    };
}

void WorldController::onEncounterXEnemy() {
    qDebug() << "Encountered an XEnemy!" << "\n";
    auto& protagonist = currentWorld->getProtagonists()[0];
    if (protagonist->getHealth() > 0) {
        protagonist->attack();
        float damage = 10.0f;
        currentWorld->currentXEnemy->takeDamage(damage);
        currentWorld->setAffectedTiles(true, currentWorld->currentXEnemy->getThunderLevel());
    } else {
        protagonist->setHealth(0);
    }
}


void WorldController::onEncounterAffectedTiles(){
    qDebug() << "Encountered a affected tile!" << "\n";
    if (currentWorld->getProtagonists()[0]->getHealth() > 0) {
        float damage = 0.2f;
        currentWorld->getProtagonists()[0]->takeDamage(damage);
        qDebug() << "Health: " << currentWorld->getProtagonists()[0]->getHealth() << "\n";
    }
    else {
        currentWorld->getProtagonists()[0]->setHealth(0);
    }
}


void WorldController::moveProtagonist(Direction direction) {
    // Get the current position of the protagonist
    coordinate currentPosition = currentWorld->getProtagonists()[0]->getPosition();

    // Calculate the new position based on direction
    int newX = currentPosition.xCoordinate;
    int newY = currentPosition.yCoordinate;

    switch (direction) {
    case UP:    newY--; break;
    case DOWN:  newY++; break;
    case LEFT:  newX--; break;
    case RIGHT: newX++; break;
    }

    // Clamp the new position to ensure it's within the world boundaries
    if (newX >= 0 && newX < currentWorld->getCols() && newY >= 0 && newY < currentWorld->getRows()) {
        // Move the protagonist
        currentWorld->getProtagonists()[0]->move(newX - currentPosition.xCoordinate, newY - currentPosition.yCoordinate);

        // Perform checks after movement
        if (currentWorld->getProtagonists()[0]->getEnergy() > 0) {
            currentWorld->getProtagonists()[0]->setEnergy(currentWorld->getProtagonists()[0]->getEnergy() - currentWorld->valueEnergyComsumingTiles(currentWorld->getProtagonists()[0]->getPosition()));
        }

        int tempEnemy = flagEnemy;
        flagEnemy = currentWorld->getEnemies().size() - currentWorld->getEnemyCounts();
        if (tempEnemy != flagEnemy) {
            currentWorld->getProtagonists()[0]->setEnergy(currentWorld->getProtagonists()[0]->getEnergy() + 10);
            if (currentWorld->getProtagonists()[0]->getEnergy() > 100) {
                currentWorld->getProtagonists()[0]->setEnergy(100);
            }
        }

        int tempPEnemy = flagPEnemy;
        flagPEnemy = currentWorld->getPEnemies().size() - currentWorld->getPEnemyCounts();
        if (tempPEnemy != flagPEnemy) {
            currentWorld->getProtagonists()[0]->setEnergy(currentWorld->getProtagonists()[0]->getEnergy() + 30);
            if (currentWorld->getProtagonists()[0]->getEnergy() > 100) {
                currentWorld->getProtagonists()[0]->setEnergy(100);
            }
        }

        int tempXEnemy = flagXEnemy;
        flagXEnemy = currentWorld->getXEnemies().size() - currentWorld->getXEnemyCounts();
        if (tempXEnemy != flagXEnemy) {
            currentWorld->getProtagonists()[0]->setEnergy(currentWorld->getProtagonists()[0]->getEnergy() + 50);
            if (currentWorld->getProtagonists()[0]->getEnergy() > 100) {
                currentWorld->getProtagonists()[0]->setEnergy(100);
            }
            currentWorld->getProtagonists()[0]->setHealth(currentWorld->getProtagonists()[0]->getHealth() + 5);
            if (currentWorld->getProtagonists()[0]->getHealth() > 5) {
                currentWorld->getProtagonists()[0]->setHealth(5);
            }
        }

        currentPosition = currentWorld->getProtagonists()[0]->getPosition();
        handleEncounters(currentPosition);

        emit updateprotagonistPosition(0);
    }
    playerReachedExit();

}

void WorldController::moveProtagonist(coordinate coord) {
    int x = coord.xCoordinate;
    int y = coord.yCoordinate;
    // Validate the x, y coordinates
    if (x < 0 || x >= currentWorld->getCols() || y < 0 || y >= currentWorld->getRows()) {
        // Handle invalid coordinates
        return;
    }
    // Move the protagonist to the specified coordinates
    currentWorld->getProtagonists()[0]->setPosition({x, y});

    // Handle encounters and other logic after movement
    coordinate currentPosition = currentWorld->getProtagonists()[0]->getPosition();
    handleEncounters(currentPosition);

    emit updateprotagonistPosition(0);
}

void WorldController::handleEncounters(const coordinate& position) {
    if (currentWorld->isEnemy(position)) {
        onEncounterEnemy();
    } else if (currentWorld->isHealthPack(position)) {
        onEncounterHealthPack();
    } else if (currentWorld->isPEnemy(position)) {
        onEncounterPEnemy();
    } else if (currentWorld->isXEnemy(position)) {
        onEncounterXEnemy();
    } else if (currentWorld->isAffectedTiles(position)) {
        onEncounterAffectedTiles();
    }
}

void WorldController::playerReachedExit() {
    // avoid going out of bound
    if (currentWorld->getProtagonists()[0]->getPosition() == currentWorld->getExit()) {
        if((currentWorldIndex + 1) > worlds.size()){
            qDebug() << "no next level. worlds size: " << worlds.size() << "currentWorldIndex: " << currentWorldIndex;
            return;
        }else if (currentWorldIndex == (worlds.size()-1)){
            emit gameWon();
            return;
        }
        auto protagonists = currentWorld->removeProtagonists();
        // Update the protagonist's position to the start position of the new world
        currentWorldIndex+= 1;
        currentWorld = worlds[currentWorldIndex].get();
        coordinate newStartPos = currentWorld->getStart();
        for (auto& prot : protagonists) {
            prot->setPosition(newStartPos);
        }
        currentWorld->addProtagonist(std::move(protagonists));
        emit updateLevel();
        qDebug() << "LevelSwitched!" << "\n";
    }
    else if(currentWorld->getProtagonists()[0]->getPosition() == currentWorld->getStart()) {
        if(static_cast<int>(currentWorldIndex - 1) == -1){
            qDebug() << "no next level. worlds size: " << worlds.size() << "currentWorldIndex: " << currentWorldIndex;
            return;
        }
        auto protagonists = currentWorld->removeProtagonists();
        // Update the protagonist's position to the start position of the new world
        currentWorldIndex-= 1;
        currentWorld = worlds[currentWorldIndex].get();
        coordinate newStartPos = currentWorld->getExit();
        for (auto& prot : protagonists) {
            prot->setPosition(newStartPos);
        }
        currentWorld->addProtagonist(std::move(protagonists));
        emit updateLevel();
    }
}


/**
 * start and exit position functions
 */


const std::vector<std::unique_ptr<WorldModel>>& WorldController::getWorlds() const {
    return worlds;
}

const WorldModel& WorldController::getCurrentWorld() const {
    if (!currentWorld) {
        throw std::runtime_error("Current world is not initialized.");
    }
    return *currentWorld;
}

void WorldController::setHeuristicWeight(float value){
    qDebug() << "Heuristic:" << value;
    heuristicWeight = value;
}

void WorldController::autoplay(){
    Comparator<node> comparator = [](const node& a, const node& b) {
        return (a.f) > (b.f);  // Assuming you want the node with the lowest 'f' value on top
    };
    // Brings the protagonist from portal to portal the fastest
    PathFinder<node,coordinate> pathFinder(currentWorld->nodes, currentWorld->getProtagonists()[0]->getPositionValue(), currentWorld->getExitValue(), comparator, this->getRows(), heuristicWeight);

    std::vector<int> result = pathFinder.A_star();
    qDebug() << "Path to destination:" << result;
    for (int move : result) {
        qDebug() << move << "path";
        coordinate currentPos = currentWorld->getProtagonists()[0]->getPosition();
        switch(move){
            case 0:
                currentPos.yCoordinate -= 1;
                moveProtagonistWithDelay(UP);
                break;
            case 1:
                currentPos.yCoordinate -= 1;
                currentPos.xCoordinate += 1;
                moveProtagonistWithDelay(RIGHT);
                moveProtagonistWithDelay(UP);
                break;
            case 2:
                currentPos.xCoordinate += 1;
                moveProtagonistWithDelay(RIGHT);
                break;
            case 3:
                currentPos.yCoordinate += 1;
                currentPos.xCoordinate += 1;
                moveProtagonistWithDelay(RIGHT);
                moveProtagonistWithDelay(DOWN);
                break;
            case 4:
                currentPos.yCoordinate += 1;
                moveProtagonistWithDelay(DOWN);
                break;
            case 5:
                currentPos.yCoordinate += 1;
                currentPos.xCoordinate -= 1;
                moveProtagonistWithDelay(DOWN);
                moveProtagonistWithDelay(LEFT);
                break;
            case 6:
                currentPos.xCoordinate -= 1;
                moveProtagonistWithDelay(LEFT);
                break;
            case 7:
                currentPos.yCoordinate -= 1;
                currentPos.xCoordinate -= 1;
                moveProtagonistWithDelay(LEFT);
                moveProtagonistWithDelay(UP);
                break;
            default:
                // Handle unexpected move values
                break;
        }
        currentWorld->getTiles().at(currentPos.yCoordinate*getCols()+currentPos.xCoordinate)->setState(MOVING);
    }
}

void WorldController::moveProtagonistWithDelay(Direction direction) {
    moveProtagonist(direction);

    // Introduce a delay using QTimer
    QTimer timer;
    connect(&timer, &QTimer::timeout, [&]() {
        // Code to be executed after the delay
        timer.stop();  // Stop the timer after the delay
    });

    // Set the delay time (adjust as needed, 1000 = 1 second)
    timer.start(100);  // 1000 milliseconds = 1 second
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
}





