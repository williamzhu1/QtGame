#ifndef ENEMYGRAPHICSITEM_H
#define ENEMYGRAPHICSITEM_H

#include "EntityGraphicsItem.h"
#include "Model/EnemyModel.h"

class EnemyGraphicsItem : public EntityGraphicsItem {
public:
    EnemyGraphicsItem(const EnemyModel& enemyModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

private:
    QString baseFramesDir;

    static std::vector<QPixmap> sharedIdleFrames;
    static std::vector<QPixmap> sharedMoveFrames;
    static std::vector<QPixmap> sharedHurtFrames;
    static std::vector<QPixmap> sharedDyingFrames;
    static std::vector<QPixmap> sharedAttackFrames;
    static std::vector<QPixmap> sharedHealFrames;

    static bool imagesLoaded;
    static void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
    static void loadSharedImages();

};

class PEnemyGraphicsItem : public EntityGraphicsItem {
public:
    PEnemyGraphicsItem(const PEnemyModel& penemyModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

private:
    QString baseFramesDir;

    static std::vector<QPixmap> sharedIdleFrames;
    static std::vector<QPixmap> sharedMoveFrames;
    static std::vector<QPixmap> sharedHurtFrames;
    static std::vector<QPixmap> sharedDyingFrames;
    static std::vector<QPixmap> sharedAttackFrames;
    static std::vector<QPixmap> sharedHealFrames;

    static bool imagesLoaded;
    static void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
    static void loadSharedImages();

};

class XEnemyGraphicsItem : public EntityGraphicsItem {
public:
    XEnemyGraphicsItem(const XEnemyModel& xenemyModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

private:
    QString baseFramesDir;

    static std::vector<QPixmap> sharedIdleFrames;
    static std::vector<QPixmap> sharedMoveFrames;
    static std::vector<QPixmap> sharedHurtFrames;
    static std::vector<QPixmap> sharedDyingFrames;
    static std::vector<QPixmap> sharedAttackFrames;
    static std::vector<QPixmap> sharedHealFrames;

    static bool imagesLoaded;
    static void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
    static void loadSharedImages();

};

#endif // ENEMYGRAPHICSITEM_H
