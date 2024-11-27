#include "EnemyModel.h"

/** Enemy Class */
EnemyModel::EnemyModel(std::unique_ptr<Enemy> enemy) : enemy(std::move(enemy)){
    strength = this->enemy->getValue();
    connect(this->enemy.get(), &Enemy::dead, this, &EnemyModel::onDead);
}

void EnemyModel::attack() {
    status = ATTACK;
    qDebug() << "enemy attack: start timer";
    float damage = 50.0f;
    QTimer::singleShot(100, this, [this, damage]() {takeDamage(damage);});
}

void EnemyModel::takeDamage(float damage) {
    strength -= damage;
    status = HURT;
    if (strength <= 0.0f) {
        QTimer::singleShot(100, this, [this]() {strength = 0.0f; enemy->setDefeated(true);});
        qDebug() << "enemy dead strength: " << strength;
    }
}

coordinate EnemyModel::getPosition() const {
    return {enemy->getXPos(), enemy->getYPos()};
}

void EnemyModel::setPosition(coordinate position) {
    enemy->setXPos(position.xCoordinate);
    enemy->setYPos(position.yCoordinate);
}

void EnemyModel::move(int deltaX, int deltaY) {
    status = MOVING;
    enemy->setXPos(enemy->getXPos() + deltaX);
    enemy->setYPos(enemy->getYPos() + deltaY);
}

// Enemy specific functions
bool EnemyModel::isDefeated() const {
    return enemy->getDefeated();
}

std::string EnemyModel::serialize() const {
    return enemy->serialize();
}

void EnemyModel::onDead(){
    status = DYING;
}


/** PEnemy Class */

PEnemyModel::PEnemyModel(std::unique_ptr<PEnemy> penemy)
    :penemy(std::move(penemy)) {
    connect(this->penemy.get(), &Enemy::dead, this, &PEnemyModel::onDead);
    connect(this->penemy.get(), &PEnemy::poisonLevelUpdated, this, &PEnemyModel::onPoisonLevelUpdated);
}

void PEnemyModel::attack() {
    status = ATTACK;
    QTimer::singleShot(100, this, [this]() {
        penemy->poison();
    });
}

void PEnemyModel::takeDamage(float newPoisonLevel) {
    status = HURT;
    QTimer::singleShot(5, this, [this, newPoisonLevel]() {
        status = IDLE;
        emit psnTilesUpdated(false, newPoisonLevel);
    });
}

coordinate PEnemyModel::getPosition() const {
    return {penemy->getXPos(), penemy->getYPos()};
}

void PEnemyModel::setPosition(coordinate position) {
    penemy->setXPos(position.xCoordinate);
    penemy->setYPos(position.yCoordinate);
}

void PEnemyModel::move(int deltaX, int deltaY) {
    status = MOVING;
    penemy->setXPos(penemy->getXPos() + deltaX);
    penemy->setYPos(penemy->getYPos() + deltaY);
    QTimer::singleShot(100, this, [this]() {
        status = IDLE;
    });
}

// PEnemy specific functions
bool PEnemyModel::isDefeated() const {
    return penemy->getDefeated();
}

float PEnemyModel::getPoisonLevel() const {
    return penemy->getPoisonLevel();
}

std::string PEnemyModel::serialize() const {
    return penemy->serialize();
}

void PEnemyModel::onDead(){
    status = DYING;
}

void PEnemyModel::onPoisonLevelUpdated(float poisonLevel){
    takeDamage(poisonLevel);
}


/** XEnemy Class */
XEnemyModel::XEnemyModel(int xPosition, int yPosition, float strength)
    : Enemy(xPosition, yPosition, strength), thunderLevel(strength){
    connect(this, &Enemy::dead, this, &XEnemyModel::onDead);
}

void XEnemyModel::attack() {
    status = ATTACK;
    QTimer::singleShot(200, this, [this]() {
        releaseThunder();
        if (!getDefeated()) {
            // xenemy is still alive
            status = IDLE;
        }
    });
}

void XEnemyModel::takeDamage(float damage) {
    status = HURT;
    std::random_device rd;  // Seed with a real random value, if available
    std::mt19937 eng(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distr(-5, 5);
    move(distr(eng), distr(eng));

    QTimer::singleShot(100, this, [this, damage]() {
        this->thunderLevel-= damage;
        this->attack();
        qDebug()<< "currentXEnemey attack";
    });
}

coordinate XEnemyModel::getPosition() const {
    return {getXPos(), getYPos()}; // Replace with actual position variables
}

void XEnemyModel::setPosition(coordinate position) {
    setXPos(position.xCoordinate);
    setYPos(position.yCoordinate);
}

void XEnemyModel::move(int deltaX, int deltaY) {
    status = MOVING;
    int upperBound = 29;
    int lowerBound = 0;
    int newX = getXPos() + deltaX;
    int newY = getXPos() + deltaX;
    if (newX < lowerBound) {
        newX = lowerBound;
    }else if (newX > upperBound){
        newX = upperBound;
    }
    if (newY < lowerBound){
        newY = lowerBound;
    }else if (newY > upperBound){
        newY = upperBound;
    }
    setXPos(newX);
    setYPos(newY);
}

// In your XEnemyModel implementation file, modify releaseThunder():
bool XEnemyModel::releaseThunder() {
   // Increment the call counter
   thunderCallCount++;

   // Thunder logic
   thunderLevel -= 5.0f;
   emit thunderLevelUpdated(true, thunderLevel);

   // Check for defeat
   if (thunderLevel <= 0.0f) {
        thunderLevel = 0.0f;
        setDefeated(true);
        return false;
   }

   if (thunderCallCount < 2) {
        // Schedule the next call with a random delay
        int delay = rand() % 5; // Random delay between 0 and 4 seconds
        QTimer::singleShot(delay * 1000, this, SLOT(releaseThunder()));
        return true;
   } else {
        // Reset the call counter for future attacks
        thunderCallCount = 0;
        return false;
   }
}


float XEnemyModel::getThunderLevel() const {
    return thunderLevel;
}

bool XEnemyModel::isDefeated() const {
    return getDefeated();
}


std::string XEnemyModel::serialize() {
    std::stringstream res;
    res << Enemy::serialize() << "," << thunderLevel;
    return res.str();
}

void XEnemyModel::onDead(){
    status = DYING;
}



