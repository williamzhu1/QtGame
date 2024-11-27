#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include <memory>
#include "Entity.h"

class EnemyModel : public QObject, public Entity {
    Q_OBJECT
public:
    EnemyModel(std::unique_ptr<Enemy> enemy); // Constructor declaration
    void attack() override;
    void takeDamage(float damage) override;
    coordinate getPosition() const override;
    void setPosition(coordinate position) override;
    void move(int deltaX, int deltaY) override;

    // Enemy specific functions
    bool isDefeated() const;
    void setDefeated(bool defeated);
    std::string serialize() const;

public slots:
    void onDead();

protected:
    std::unique_ptr<Enemy> enemy;
    float strength;
};

class PEnemyModel : public QObject, public Entity {
    Q_OBJECT
public:
    explicit PEnemyModel(std::unique_ptr<PEnemy> penemy);

    // Override Enemy's functions if necessary and add PEnemy specific functions
    void attack() override;
    void takeDamage(float newPoisonLevel) override;
    coordinate getPosition() const override;
    void setPosition(coordinate position) override;
    void move(int deltaX, int deltaY) override;

    // PEnemy specific functions
    bool isDefeated() const;
    float getPoisonLevel() const;
    std::string serialize() const;

public slots:
    void onDead();
    void onPoisonLevelUpdated(float poisonLevel);

    signals:
        void psnTilesUpdated(bool xenemyType, float newPoisonLevel);

    private:
        std::unique_ptr<PEnemy> penemy;
};

class XEnemyModel : public Enemy, public Entity {
        Q_OBJECT
    public:
        explicit XEnemyModel(int xPosition, int yPosition, float strength);
        ~XEnemyModel() override = default;
        void attack() override;
        void takeDamage(float damage) override;
        coordinate getPosition() const override;
        void setPosition(coordinate position) override;
        void move(int deltaX, int deltaY) override;

        // XEnemy specific functions
        bool isDefeated() const;
        float getThunderLevel() const;
        std::string serialize() override;

    public slots:
        void onDead();
        bool releaseThunder();

    signals:
        void thunderLevelUpdated(bool xenemyType, int value);

    private:
        float thunderLevel;
        int thunderCallCount = 0;
};

#endif // ENEMYMODEL_H
