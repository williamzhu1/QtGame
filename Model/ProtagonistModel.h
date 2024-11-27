#ifndef PROTAGONISTMODEL_H
#define PROTAGONISTMODEL_H

#include <memory>
#include "Entity.h"

class ProtagonistModel : public QObject, public Entity {
    Q_OBJECT
    public:
        explicit ProtagonistModel(std::unique_ptr<Protagonist> protagonist); // Constructor declaration

        void attack() override;
        void takeDamage(float damage) override;
        coordinate getPosition() const override;
        coordinate* getPositionValue();
        void setPosition(coordinate position) override;
        void move(int deltaX, int deltaY) override;

        // Additional functionalities specific to ProtagonistModel
        int getHealth() const;
        void setHealth(float health);
        float getEnergy() const;
        void setEnergy(float energy);
        std::string serialize() const;

    public slots:
        void onPosChanged(int x, int y);
        void onHealthChanged(int h);
        void oneEnergyChanged(int e);

    signals:
        void protagonistDead();
        void healthUpdate();
        void energyUpdate();

    private:
        std::unique_ptr<Protagonist> protagonist;
        coordinate protagonistPos = coordinate(0,0);
};

#endif // PROTAGONISTMODEL_H
