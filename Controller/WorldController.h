#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "Model/structs.h"
#include "Model/WorldModel.h"
#include "pathfinder.h"
#include <QEventLoop>
#include <QTimer>
#include <iostream>
#include <map>
#include <QObject>
#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>
#include <random>

class WorldController : public QObject
{
    Q_OBJECT

public:
    static WorldController& getInstance() {
        static WorldController instance;
        return instance;
    }

    WorldController(WorldController const&) = delete;
    void operator=(WorldController const&) = delete;

    void createWorld(QString map, int gameNumberOfPlayers, int gameDifficultyIdx, float pRatio);

    int getRows() const;
    int getCols() const;

    int getNumOfProtagonists() const;
    int getDifficultyIdx() const;

    void deleteEnemy(coordinate);
    void deletePsnTile(coordinate);

    void playerReachedExit();

    const std::vector<std::unique_ptr<WorldModel>>& getWorlds() const;
    const WorldModel& getCurrentWorld() const;

    void autoplay();
    void setHeuristicWeight(float value);

    void moveProtagonist(Direction direction);
    void moveProtagonist(coordinate coord);
    void moveProtagonistWithDelay(Direction direction);

signals:
    void updateprotagonistPosition(int protagonistIndex);
    void updateLevel();
    void gameWon();
    void updateHealthAndEnergy();
    void onProtagonistDead();
    void onHealthAndEnergyUpdate();

private:
    WorldController();

    std::vector<std::unique_ptr<WorldModel>> worlds;
    WorldModel* currentWorld;
    int difficultyIdx;
    size_t currentWorldIndex;

    void handleEncounters(const coordinate& position);
    void onEncounterHealthPack();
    void onEncounterEnemy();
    void onEncounterPEnemy();
    void onEncounterXEnemy();
    void onEncounterAffectedTiles();

    int flagEnemy = 0;
    int flagPEnemy = 0;
    int flagXEnemy = 0;

    float heuristicWeight = 0;

    bool firstWorld = true;

};

#endif // WORLDCONTROLLER_H
