#include "EnemyGraphicsItem.h"
#include <QDir>

//QString EntityGraphicsItem::baseFramesDir = "images/enemy_golem/PNG Sequences/";

bool EnemyGraphicsItem::imagesLoaded = false;
std::vector<QPixmap> EnemyGraphicsItem::sharedIdleFrames;
std::vector<QPixmap> EnemyGraphicsItem::sharedMoveFrames;
std::vector<QPixmap> EnemyGraphicsItem::sharedHurtFrames;
std::vector<QPixmap> EnemyGraphicsItem::sharedDyingFrames;
std::vector<QPixmap> EnemyGraphicsItem::sharedAttackFrames;
std::vector<QPixmap> EnemyGraphicsItem::sharedHealFrames;

EnemyGraphicsItem::EnemyGraphicsItem(const EnemyModel& enemyModel, const QString& baseFramesDir, QGraphicsRectItem* parent)
    : EntityGraphicsItem(enemyModel, parent), baseFramesDir(baseFramesDir){
    loadSharedImages();
    idleFrames = sharedIdleFrames;
    moveFrames = sharedMoveFrames;
    hurtFrames = sharedHurtFrames;
    dyingFrames = sharedDyingFrames;
    attackFrames = sharedAttackFrames;
    healFrames = sharedHealFrames;
}

void EnemyGraphicsItem::loadSharedImages() {
    if(!imagesLoaded){
        QString baseFramesDir = ":/images/enemy_golem/PNG Sequences/";
        // Load the frames for each animation state
        loadFramesFromDirectory(baseFramesDir + "Idle/", sharedIdleFrames);
        loadFramesFromDirectory(baseFramesDir + "Moving/", sharedMoveFrames);
        loadFramesFromDirectory(baseFramesDir + "Hurt/", sharedHurtFrames);
        loadFramesFromDirectory(baseFramesDir + "Dying/", sharedDyingFrames);
        loadFramesFromDirectory(baseFramesDir + "Attack/", sharedAttackFrames);
        loadFramesFromDirectory(baseFramesDir + "Heal/", sharedHealFrames);
        imagesLoaded = true;
    }
}

void EnemyGraphicsItem::loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames) {
    QDir dir(dirPath);
    QStringList fileNames = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
    for (const QString& fileName : fileNames) {
        QPixmap frame(dirPath + fileName);
        if (!frame.isNull()) {
            // Pre-scale the image here
            QPixmap scaledFrame = frame.scaled(EntityGraphicsItem::commonWidth, EntityGraphicsItem::commonHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            frames.push_back(scaledFrame);
        } else {
            qDebug() << "Failed to load frame:" << dirPath + fileName;
        }
    }
}

bool PEnemyGraphicsItem::imagesLoaded = false;
std::vector<QPixmap> PEnemyGraphicsItem::sharedIdleFrames;
std::vector<QPixmap> PEnemyGraphicsItem::sharedMoveFrames;
std::vector<QPixmap> PEnemyGraphicsItem::sharedHurtFrames;
std::vector<QPixmap> PEnemyGraphicsItem::sharedDyingFrames;
std::vector<QPixmap> PEnemyGraphicsItem::sharedAttackFrames;
std::vector<QPixmap> PEnemyGraphicsItem::sharedHealFrames;

PEnemyGraphicsItem::PEnemyGraphicsItem(const PEnemyModel& penemyModel, const QString& baseFramesDir, QGraphicsRectItem* parent)
    : EntityGraphicsItem(penemyModel, parent), baseFramesDir(baseFramesDir){
    loadSharedImages();
    idleFrames = sharedIdleFrames;
    moveFrames = sharedMoveFrames;
    hurtFrames = sharedHurtFrames;
    dyingFrames = sharedDyingFrames;
    attackFrames = sharedAttackFrames;
    healFrames = sharedHealFrames;
}

void PEnemyGraphicsItem::loadSharedImages() {

    if(!imagesLoaded){
        QString baseFramesDir = ":/images/penemy_wraith/PNG Sequences/";
        // Load the frames for each animation state
        loadFramesFromDirectory(baseFramesDir + "Idle/", sharedIdleFrames);
        loadFramesFromDirectory(baseFramesDir + "Moving/", sharedMoveFrames);
        loadFramesFromDirectory(baseFramesDir + "Hurt/", sharedHurtFrames);
        loadFramesFromDirectory(baseFramesDir + "Dying/", sharedDyingFrames);
        loadFramesFromDirectory(baseFramesDir + "Attack/", sharedAttackFrames);
        loadFramesFromDirectory(baseFramesDir + "Heal/", sharedHealFrames);
        imagesLoaded = true;
    }
}

void PEnemyGraphicsItem::loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames) {
    QDir dir(dirPath);
    QStringList fileNames = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
    for (const QString& fileName : fileNames) {
        QPixmap frame(dirPath + fileName);
        if (!frame.isNull()) {
            // Pre-scale the image here
            QPixmap scaledFrame = frame.scaled(EntityGraphicsItem::commonWidth, EntityGraphicsItem::commonHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            frames.push_back(scaledFrame);
        } else {
            qDebug() << "Failed to load frame:" << dirPath + fileName;
        }
    }
}

bool XEnemyGraphicsItem::imagesLoaded = false;
std::vector<QPixmap> XEnemyGraphicsItem::sharedIdleFrames;
std::vector<QPixmap> XEnemyGraphicsItem::sharedMoveFrames;
std::vector<QPixmap> XEnemyGraphicsItem::sharedHurtFrames;
std::vector<QPixmap> XEnemyGraphicsItem::sharedDyingFrames;
std::vector<QPixmap> XEnemyGraphicsItem::sharedAttackFrames;
std::vector<QPixmap> XEnemyGraphicsItem::sharedHealFrames;

XEnemyGraphicsItem::XEnemyGraphicsItem(const XEnemyModel& xenemyModel, const QString& baseFramesDir, QGraphicsRectItem* parent)
    : EntityGraphicsItem(xenemyModel, parent), baseFramesDir(baseFramesDir){
    loadSharedImages();
    idleFrames = sharedIdleFrames;
    moveFrames = sharedMoveFrames;
    hurtFrames = sharedHurtFrames;
    dyingFrames = sharedDyingFrames;
    attackFrames = sharedAttackFrames;
    healFrames = sharedHealFrames;
}

void XEnemyGraphicsItem::loadSharedImages() {
    if(!imagesLoaded){
        QString baseFramesDir = ":/images/xenemy_wraith/PNG Sequences/";
        // Load the frames for each animation state
        loadFramesFromDirectory(baseFramesDir + "Idle/", sharedIdleFrames);
        loadFramesFromDirectory(baseFramesDir + "Moving/", sharedMoveFrames);
        loadFramesFromDirectory(baseFramesDir + "Hurt/", sharedHurtFrames);
        loadFramesFromDirectory(baseFramesDir + "Dying/", sharedDyingFrames);
        loadFramesFromDirectory(baseFramesDir + "Attack/", sharedAttackFrames);
        loadFramesFromDirectory(baseFramesDir + "Heal/", sharedHealFrames);
        imagesLoaded = true;
    }
}

void XEnemyGraphicsItem::loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames) {
    QDir dir(dirPath);
    QStringList fileNames = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
    for (const QString& fileName : fileNames) {
        QPixmap frame(dirPath + fileName);
        if (!frame.isNull()) {
            // Pre-scale the image here
            QPixmap scaledFrame = frame.scaled(EntityGraphicsItem::commonWidth, EntityGraphicsItem::commonHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            frames.push_back(scaledFrame);
        } else {
            qDebug() << "Failed to load frame:" << dirPath + fileName;
        }
    }
}
