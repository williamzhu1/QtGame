#ifndef WORLDMODEL_H
#define WORLDMODEL_H
#include <QObject>
#include <QDebug>
#include <QString>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include "Model/world.h"
#include "Model/structs.h"
#include "Model/TileModel.h"
#include "Model/EnemyModel.h"
#include "Model/ProtagonistModel.h"
#include "Model/Entity.h"

class WorldModel: public QObject{
     Q_OBJECT
public:
     explicit WorldModel(QString map, int nrOfEnemies, int nrOfHealthpacks, float pRatio, bool firstWorld);
     int getRows() const;
     int getCols() const;

     void addProtagonist(std::vector<std::unique_ptr<ProtagonistModel>>);
     std::vector<std::unique_ptr<ProtagonistModel>> removeProtagonists();
     const std::vector<std::unique_ptr<TileModel>>& getTiles() const;
     const std::vector<std::unique_ptr<TileModel>>& getHealthPacks() const;
     const std::vector<std::unique_ptr<EnemyModel>>& getEnemies() const;
     const std::vector<std::unique_ptr<PEnemyModel>>& getPEnemies() const;
     const std::vector<std::unique_ptr<XEnemyModel>>& getXEnemies() const;
     const std::vector<std::unique_ptr<ProtagonistModel>>& getProtagonists() const;

     bool isHealthPack(coordinate);
     bool isAffectedTiles(coordinate);
     float valueEnergyComsumingTiles(coordinate);
     int getEnemyCounts();
     int getPEnemyCounts();
     int getXEnemyCounts();
     bool isEnemy(coordinate);
     bool isPEnemy(coordinate);
     bool isXEnemy(coordinate);

     int getNumOfProtagonists() const;
     int getDifficultyIdx() const;

     void deleteEnemy(coordinate);
     void deletePsnTile(coordinate);
     void removeHealthpack(coordinate);

     coordinate getStart() const;
     coordinate getExit() const;
     coordinate* getStartValue();
     coordinate* getExitValue();
     coordinate findNearestHealthPack() const;
     coordinate findNearestEnemy() const;
     coordinate findNearestPEnemy() const;
     coordinate findNearestXEnemy() const;

     std::vector<node> nodes;
     ProtagonistModel* currentProtagonist;
     EnemyModel* currentEnemy;
     PEnemyModel* currentPEnemy;
     XEnemyModel* currentXEnemy;
     TileModel* currentHealthpack;

public slots:
     void setAffectedTiles(bool xenemyType, float value);

private:
     std::unique_ptr<World> world;
     int rows;
     int cols;
     coordinate exit = coordinate(29,29);
     coordinate start = coordinate(0,0);
     std::vector<std::unique_ptr<TileModel>> tiles;
     std::vector<std::unique_ptr<TileModel>> healthPacks;
     std::vector<std::unique_ptr<EnemyModel>> enemies;
     std::vector<std::unique_ptr<PEnemyModel>> penemies;
     std::vector<std::unique_ptr<XEnemyModel>> xenemies;
     std::vector<std::unique_ptr<ProtagonistModel>> protagonists;
};

#endif // WORLDMODEL_H
