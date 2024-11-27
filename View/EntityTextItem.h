// EntityTextItem.h
#ifndef ENTITYTEXTITEM_H
#define ENTITYTEXTITEM_H

#include <QString>
#include "Model/Entity.h"
#include "Model/TileModel.h"
#include "Model/ProtagonistModel.h"
#include "Model/EnemyModel.h"


class EntityTextItem {
public:
    EntityTextItem(const Entity& entity) : entity(entity)/*, defaultTextRepresentation("?")*/ {
        // Default representation for a generic entity
        textRepresentation = "?";
    }

    virtual ~EntityTextItem() {}

    const QString getTextRepresentation() {
        QString color;
        switch (entity.getState()) {
        case IDLE:
            color = "purple";
            break;
        case ATTACK:
            color = "red";
            break;
        case HURT:
            color = "blue";
            break;
        case MOVING:
            color = "pink";
            break;
        case DYING:
            color = "grey";
            break;
        default:
            color = "black";
            break;
        }
        return QString("<font color='%1'>%2</font>").arg(color, textRepresentation);
    }

    coordinate getPosition() const {
        return entity.getPosition();
    }

    void setIndex(int newIndex) {
        index = newIndex;
    }

    int getIndex() const {
        return index;
    }

    const Entity& getEntity() const{
        return entity;
    }

protected:
    const Entity& entity; // Presuming Entity has position and other relevant attributes
    QString textRepresentation;

private:
    int index; // Stores the index position within the grid
};


class TileTextItem : public EntityTextItem {
public:
    TileTextItem(const TileModel& tile) : EntityTextItem(tile) {
        textRepresentation = "°";
    }

};

class HPTextItem : public EntityTextItem {
public:
    HPTextItem(const TileModel& hp) : EntityTextItem(hp) {
        // Set a text representation specific to tiles
        textRepresentation = "H";
    }

};

class EnemyTextItem : public EntityTextItem {
public:
    EnemyTextItem(const EnemyModel& enemy) : EntityTextItem(enemy) {
        // Set a text representation specific to enemies
        textRepresentation = "E";
    }
};

class PEnemyTextItem : public EntityTextItem {
public:
    PEnemyTextItem(const PEnemyModel& penemy) : EntityTextItem(penemy) {
        // Set a text representation specific to enemies
        textRepresentation = "θ";
    }
};

class XEnemyTextItem : public EntityTextItem {
public:
    XEnemyTextItem(const XEnemyModel& xenemy) : EntityTextItem(xenemy) {
        // Set a text representation specific to enemies
        textRepresentation = "X";
    }
};


class ProtagonistTextItem : public EntityTextItem {
public:
    ProtagonistTextItem(const ProtagonistModel& protagonist) : EntityTextItem(protagonist) {
        // Set a text representation specific to protagonists
        textRepresentation = "P";
    }
};

#endif // ENTITYTEXTITEM_H
