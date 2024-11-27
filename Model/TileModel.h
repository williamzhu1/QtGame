#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Entity.h"
#include "TileObserver.h"
#include <memory>

class TileModel : public QObject, public Entity {
    Q_OBJECT
public:
    /** TileModel is a model wrapper for Tile and HealthPack
     *  to be used in TileGraphicsItem
    **/
    TileModel(std::unique_ptr<Tile> tile) : tile(std::move(tile)) {
    }
    
    // Functions to expose Tile functionality
    void setPosition(coordinate position) override;
    coordinate getPosition() const override;
    float getValue() const;
    void setValue(float value);
    std::string serialize() const;
    void attack() override;
    void takeDamage(float strength) override;
    void setState(state newStatus) override{
        status = newStatus;
        notifyObservers();
    }

    // Functions specific to tile
    inline bool isPoisoned() const { return poisoned; }
    inline bool isThundered() const {return thundered; }
    inline void setAffectedType(bool xenemyType) { this->poisoned = !xenemyType; this->thundered = xenemyType; }

    inline void addObserver(TileObserver* observer) const {
        observers.push_back(observer);
    }

    inline void removeObserver(TileObserver* observer) const {
        // Remove observer from the vector
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    inline void notifyObservers() const {
        for (TileObserver* observer : observers) {
            observer->onTileChanged();
        }
    }

    // Functions unused by TileModel
    void move(int deltaX, int deltaY) override{
        Q_UNUSED(deltaX);
        Q_UNUSED(deltaY);
    }

    mutable std::vector<TileObserver*> observers;

private:
    std::unique_ptr<Tile> tile;
    bool poisoned {false};
    bool thundered {false};
    int energyValue = 0;
};

#endif // TILEMODEL_H
