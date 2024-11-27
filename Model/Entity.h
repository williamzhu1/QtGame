#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <sstream> // Include for std::stringstream
#include <random>
#include "Model/structs.h"
#include "world.h"

class Entity/*: public QObject*/{
//    Q_OBJECT
public:
    Entity() : status(IDLE) {}
    virtual ~Entity() {}

    // Pure virtual functions that must be implemented by any subclass
    virtual void attack() = 0;
    virtual void takeDamage(float damage) = 0;
    virtual coordinate getPosition() const = 0;
    virtual void setPosition(coordinate position) = 0;
    virtual void move(int deltaX, int deltaY) = 0;
    state getState() const {
        return status;
    }

    virtual void setState(state newStatus) {
        status = newStatus;
    }
protected:
    state status;
};

#endif // ENTITY_H
