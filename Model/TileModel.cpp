#include "TileModel.h"

// Get the position of the tile
coordinate TileModel::getPosition() const {
    return coordinate(tile->getXPos(), tile->getYPos());
}

// Set the position of the tile
void TileModel::setPosition(coordinate position) {
    tile->setXPos(position.xCoordinate);
    tile->setYPos(position.yCoordinate);
}

// Get the value of the tile
float TileModel::getValue() const {
    return tile->getValue();
}

// Set the value of the tile
void TileModel::setValue(float value) {
    tile->setValue(value);
}

// Serialize the tile data
std::string TileModel::serialize() const {
    return tile->serialize();
}

void TileModel::attack() {
    if(poisoned){
        // afftected by penemy
        status = ATTACK;
    }else{
        //affected by xenemy
        status = HURT;
    }
    notifyObservers();
}

void TileModel::takeDamage(float strength) {
    if(strength > 10){
        attack();
    }else{
        QTimer::singleShot(6000, this, [this]() {
            status = DYING;
        });
    }
}
