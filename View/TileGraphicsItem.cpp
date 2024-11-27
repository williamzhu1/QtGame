#include "TileGraphicsItem.h"

bool TileGraphicsItem::imagesLoaded = false;
std::vector<QPixmap> TileGraphicsItem::sharedIdleFrames;
std::vector<QPixmap> TileGraphicsItem::sharedPathFoundFrames;
std::vector<QPixmap> TileGraphicsItem::sharedThunderedFrames;
std::vector<QPixmap> TileGraphicsItem::sharedPoisonedFrames;


TileGraphicsItem::TileGraphicsItem(const TileModel& tileModel, const QString& baseFramesDir, QGraphicsRectItem* parent)
    : EntityGraphicsItem(tileModel, parent), tileModel(tileModel), baseFramesDir(baseFramesDir){
    this->animationTimer->stop();
    tileModel.addObserver(this);
    loadSharedImages();
    idleFrames = sharedIdleFrames;
    moveFrames = sharedPathFoundFrames;
    hurtFrames = sharedThunderedFrames;
    attackFrames = sharedPoisonedFrames;
}

void TileGraphicsItem::loadSharedImages() {
    if (!imagesLoaded) {
        QString baseFramesDir = ":/images/tiles/";
        loadFramesFromDirectory(baseFramesDir + "Idle/", sharedIdleFrames);
        loadFramesFromDirectory(baseFramesDir + "Moving/", sharedPathFoundFrames);
        loadFramesFromDirectory(baseFramesDir + "Hurt/", sharedThunderedFrames);
        loadFramesFromDirectory(baseFramesDir + "Attack/", sharedPoisonedFrames);
        imagesLoaded = true;
    }
}

void TileGraphicsItem::loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames) {
    QDir dir(dirPath);
    if(!dir.exists()){
        return;
    }
    QStringList fileNames = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
    for (const QString& fileName : fileNames) {
        QPixmap frame(dirPath + fileName);
        if (!frame.isNull()) {
            // Pre-scale the image here
            QPixmap scaledFrame = frame.scaled(EntityGraphicsItem::tileWidth, EntityGraphicsItem::tileHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            frames.push_back(scaledFrame);
        } else {
            qDebug() << "Failed to load frame:" << dirPath + fileName;
        }
    }
}

QRectF TileGraphicsItem::boundingRect() const {
    return QRectF(0, 0, tileWidth, tileHeight);
}


void TileGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Calculate the backgorund index and draw the background image
    float tileVal = this->tileModel.getValue();
    int maxIndex = idleFrames.size()-1;
    if(tileVal < 1){
        int backgroundIndex = static_cast<int>((1.0 - this->tileModel.getValue()) * maxIndex) /*% maxIndex*/;
        backgroundImage = idleFrames[backgroundIndex];
    }else{
        backgroundImage = idleFrames[maxIndex];
    }

    painter->drawPixmap(0, 0, backgroundImage);

    // draw the animation frame if not null
    if(!image.isNull()){
        painter->drawPixmap(0, 0, image);
    }
}

const TileModel& TileGraphicsItem::getTileModel(){
    return tileModel;
}

void TileGraphicsItem::onTileChanged(){
    // re-activate timer
    if (!this->animationTimer->isActive()) {
        this->animationTimer->start(100);
    }
}

void TileGraphicsItem::nextFrame() {
    const state entityState = entity.getState();
    switch (entityState) {
    case IDLE:
        break;
    case MOVING:
        if(!moveFrames.empty()){
            currentFrameIndex = (currentFrameIndex + 1) % (moveFrames.size());
            if (currentFrameIndex >= moveFrames.size()-1) {
                QTimer::singleShot(1000, this, [this]() {
                    currentFrameIndex = 0;
                    this->animationTimer->stop();
                });
            }
            image = moveFrames[currentFrameIndex];
        }
        break;
    case ATTACK:
        if(!attackFrames.empty()){
            currentFrameIndex = (currentFrameIndex + 1) % attackFrames.size();
            image = attackFrames[currentFrameIndex];
        }
        break;
    case HURT:
        if(!hurtFrames.empty()){
            currentFrameIndex = (currentFrameIndex + 1) % hurtFrames.size();
            image = hurtFrames[currentFrameIndex];
        }
        break;
    case HEAL:
        break;
    case DYING:
        this->animationTimer->stop();
        break;
    }
    update();
}


/** HealpthPack Sectoin */

bool HPGraphicsItem::imagesLoaded = false;
std::vector<QPixmap> HPGraphicsItem::sharedIdleFrames;

// the naming should be changed to TileGraphicsItem

HPGraphicsItem::HPGraphicsItem(const TileModel& tileModel, const QString& baseFramesDir, QGraphicsRectItem* parent)
    : EntityGraphicsItem(tileModel, parent), tileModel(tileModel), baseFramesDir(baseFramesDir){
    this->animationTimer->stop();
    loadSharedImages();
    idleFrames = sharedIdleFrames;
}

void HPGraphicsItem::loadSharedImages() {
    if (!imagesLoaded) {
        QString baseFramesDir = ":/images/healthpack/";
        loadFramesFromDirectory(baseFramesDir + "Idle/", sharedIdleFrames);
        imagesLoaded = true;
    }
}

void HPGraphicsItem::loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames) {
    QDir dir(dirPath);
    if(!dir.exists()){
        return;
    }
    QStringList fileNames = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
    for (const QString& fileName : fileNames) {
        QPixmap frame(dirPath + fileName);
        if (!frame.isNull()) {
            // Pre-scale the image here
            QPixmap scaledFrame = frame.scaled(EntityGraphicsItem::tileWidth, EntityGraphicsItem::tileHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            frames.push_back(scaledFrame);
        } else {
            qDebug() << "Failed to load frame:" << dirPath + fileName;
        }
    }
}

QRectF HPGraphicsItem::boundingRect() const {
    return QRectF(0, 0, tileWidth, tileHeight);
}

void HPGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Calculate the backgorund index and draw the background image
    int backgroundIndex = static_cast<int>(this->tileModel.getValue() * idleFrames.size()-1) % (idleFrames.size()-1);
    backgroundImage = idleFrames[backgroundIndex];

    painter->drawPixmap(0, 0, backgroundImage);
}

/** Portal Sectoin */

bool PortalGraphicsItem::imagesLoaded = false;
std::vector<QPixmap> PortalGraphicsItem::sharedIdleFrames;

PortalGraphicsItem::PortalGraphicsItem(const TileModel& tileModel, const QString& baseFramesDir, QGraphicsRectItem* parent)
    : EntityGraphicsItem(tileModel, parent), baseFramesDir(baseFramesDir){
    loadSharedImages();
    idleFrames = sharedIdleFrames;
}

void PortalGraphicsItem::loadSharedImages() {
    if (!imagesLoaded) {
    QString baseFramesDir = ":/images/portal/";
        loadFramesFromDirectory(baseFramesDir + "Idle/", sharedIdleFrames);
        imagesLoaded = true;
    }
}

void PortalGraphicsItem::loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames) {
    QDir dir(dirPath);
    if(!dir.exists()){
        return;
    }
    QStringList fileNames = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
    for (const QString& fileName : fileNames) {
        QPixmap frame(dirPath + fileName);
        if (!frame.isNull()) {
            // Pre-scale the image here
            QPixmap scaledFrame = frame.scaled(EntityGraphicsItem::tileWidth, EntityGraphicsItem::tileHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            frames.push_back(scaledFrame);
        } else {
            qDebug() << "Failed to load frame:" << dirPath + fileName;
        }
    }
}

QRectF PortalGraphicsItem::boundingRect() const {
    return QRectF(0, 0, tileWidth, tileHeight);
}


void PortalGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // draw the animation frame if not null
    if(!image.isNull()){
        painter->drawPixmap(0, 0, image);
    }
}

