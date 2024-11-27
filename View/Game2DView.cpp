#include "Game2DView.h"

void Game2DView::setCurrentWorld(const WorldModel& world){
    this->world = &world;
}

void Game2DView::initializeView() {
    if (!scene) {
        scene = new QGraphicsScene(this);
        setScene(scene);
    }
    /** avoid adding duplicated tile observer */
    if (tileGraphicsItems.size() != 0) {
        for (auto* tileGraphicsItem : tileGraphicsItems) {
            tileGraphicsItem->getTileModel().removeObserver(tileGraphicsItem);
        }
    }
    /** clear all vectors */
    tileGraphicsItems.clear();
    healthpackGraphicsItems.clear();
    enemyGraphicsItems.clear();
    penemyGraphicsItems.clear();
    xenemyGraphicsItems.clear();
    portalGraphicsItems.clear();
    protagonistGraphicsItems.clear();
    scene->clear();


    setBackground(backgroundNumber);
    scaleEntitiesToFitView();
    // Extract entities from the WorldController
    const std::vector<std::unique_ptr<ProtagonistModel>>& protagonists = world->getProtagonists();

    /** baseFramesDir for tile is constant */
    QString tileBase = ":/images/tiles/";
    for (const auto& tile : world->getTiles()) {
        TileGraphicsItem* item = new TileGraphicsItem(*tile, tileBase);
        scene->addItem(item);
        tileGraphicsItems.push_back(item);
    }

    /** baseFramesDir for portal is constant */
    QString portalBase = ":/images/portal/";
    // compute the exit tile index and set the picture
    const std::unique_ptr<TileModel>& exit = world->getTiles().at((world->getExit().yCoordinate) * world->getCols() + (world->getExit().xCoordinate));
    const std::unique_ptr<TileModel>& start = world->getTiles().at((world->getStart().yCoordinate) * world->getCols() + (world->getStart().xCoordinate));
    PortalGraphicsItem* exitGraphicsItem = new PortalGraphicsItem(*exit, portalBase);
    PortalGraphicsItem* startGraphicsItem = new PortalGraphicsItem(*start, portalBase);
    scene->addItem(exitGraphicsItem);
    scene->addItem(startGraphicsItem);
    portalGraphicsItems.push_back(exitGraphicsItem);
    portalGraphicsItems.push_back(startGraphicsItem);

    /** baseFramesDir for healthpack is constant */
    QString healthpackBase = ":/images/healthpack/";
    for (const auto& healthPack : world->getHealthPacks()) {
        HPGraphicsItem* healthPackGraphicsItem = new HPGraphicsItem(*healthPack, healthpackBase);
        scene->addItem(healthPackGraphicsItem);
        healthpackGraphicsItems.push_back(healthPackGraphicsItem);
    }

    /** baseFramesDir for enemy is constant */
    QString enemyBase = ":/images/enemy_golem/PNG Sequences/";
    for (const auto& enemy : world->getEnemies()) {
        EnemyGraphicsItem* enemyGraphicsItem = new EnemyGraphicsItem(*enemy, enemyBase);
        scene->addItem(enemyGraphicsItem);
        enemyGraphicsItems.push_back(enemyGraphicsItem);
    }

    /** baseFramesDir for penemy is constant */
    QString penemyBase = ":/images/penemy_wraith/PNG Sequences/";
    for (const auto& penemy : world->getPEnemies()) {
        PEnemyGraphicsItem* penemyGraphicsItem = new PEnemyGraphicsItem(*penemy, penemyBase);
        scene->addItem(penemyGraphicsItem);
        penemyGraphicsItems.push_back(penemyGraphicsItem);
    }

    /** baseFramesDir for xenemy is constant */
    QString xenemyBase = ":/images/xenemy_wraith/PNG Sequences/";
    for (const auto& xenemy : world->getXEnemies()) {
        XEnemyGraphicsItem* xenemyGraphicsItem = new XEnemyGraphicsItem(*xenemy, xenemyBase);
        scene->addItem(xenemyGraphicsItem);
        xenemyGraphicsItems.push_back(xenemyGraphicsItem);
    }


    /** baseFramesDir for protagonist depends on numbers of protagonist*/
    QString pro1Base = ":/images/protagonist_fighter/";
    QString pro2Base = ":/images/protagonist_samurai/";
    QString pro3Base = ":/images/protagonist_shinobi/";
    for (size_t i = 0; i < protagonists.size(); ++i) {
        QString protagonistBase; // Variable to hold the base frame directory
        if (i == 0) {
            protagonistBase = pro1Base;
        } else if (i == 1) {
            protagonistBase = pro1Base;
        } else if (i == 2){
            protagonistBase = pro2Base;
        } else if (i == 3){
            protagonistBase = pro3Base;
        } else {
            protagonistBase = pro1Base;
        }
        ProtagonistGraphicsItem* protagonistGraphicsItem = new ProtagonistGraphicsItem(*protagonists[i], protagonistBase);
        scene->addItem(protagonistGraphicsItem);
        protagonistGraphicsItems.push_back(protagonistGraphicsItem);
    }
    scene->setSceneRect(0, 0, backgroundImage.width() + tileWidth, backgroundImage.height() + tileHeight);
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    qDebug() << "items added scene width: " << scene->width() << "items added scene height" << scene->height();
    qDebug() << "items added view width: " << this->width() << "items added view height" << this->height();

    this->update();

}

void Game2DView::updateView() {
    for (auto* item : tileGraphicsItems) {
        item->updatePosition();
    }
    for(auto* protagonistGraphicsItem : protagonistGraphicsItems) {
        if (protagonistGraphicsItem) {
            protagonistGraphicsItem->updatePosition();
        }
    }
    for (auto* healthpackGraphicsItem : healthpackGraphicsItems) {
        if (healthpackGraphicsItem) {
            healthpackGraphicsItem->updatePosition();
        }
    }
    for (auto* xenemyGraphicsItem : xenemyGraphicsItems) {
        if (xenemyGraphicsItem) {
            xenemyGraphicsItem->updatePosition();
        }
    }
    scene->setSceneRect(scene->itemsBoundingRect());
    this->update();
}

void Game2DView::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y() / 12 ;
    if (delta > 0) {
        zoomIn(std::abs(delta));  // Zoom in when the wheel is scrolled up
    }else{
        zoomOut(std::abs(delta)); // Zoom out when the wheel is scrolled down
    }
}

void Game2DView::setBackgroundNumber(int backgroundNumber){
    this->backgroundNumber = backgroundNumber;
}

void Game2DView::setBackground(int backgroundNumber) {
    // Load the background image based on the difficulty level
    switch(backgroundNumber) {
    case 1: backgroundImage = Background1; tileWidth = 30; tileHeight = 30; break;
    case 2: backgroundImage = Background2; tileWidth = 30; tileHeight = 30; break;
    case 3: backgroundImage = Background3; tileWidth = 30; tileHeight = 20; break;
    default: backgroundImage= Background1; tileWidth = 30; tileHeight = 30; break;
    }

    backgroundImage = backgroundImage.scaled(tileWidth * world->getCols(),
                                             tileHeight * world->getRows(),
                                             Qt::KeepAspectRatioByExpanding);

    // Add the background image as a pixmap item to the scene
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(backgroundImage);
    backgroundItem->setZValue(-1); // Ensure it's drawn below all other items
    scene->addItem(backgroundItem);

    // Set the scene's rectangle to the size of the resized background image
    scene->setSceneRect(0, 0, backgroundImage.width(), backgroundImage.height());
    this->update();
}

void Game2DView::zoomIn(int delta) {
    qreal maxZoomLevel = initZoomLevel * 2;
    qreal targetZoomLevel = zoomLevel + delta * zoomSpeed;
    if (targetZoomLevel > maxZoomLevel) {  // If the target zoom level is greater than the max, clamp it
        targetZoomLevel = maxZoomLevel;
        this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }

    if (zoomLevel < targetZoomLevel) {  // Only zoom in if not already at the max zoom level
        qreal factor = qPow(2.0, targetZoomLevel);
        setTransform(QTransform::fromScale(factor, factor));
        if (!protagonistGraphicsItems.empty()) {
            centerOn(protagonistGraphicsItems.front());
        }
        zoomLevel = targetZoomLevel; // Update the current zoom level
    }
    this->update();
}

void Game2DView::zoomOut(int delta) {
    qreal scale = 0.001;
    qreal minZoomLevel = (initZoomLevel * scale);
    qreal targetZoomLevel = zoomLevel - delta * zoomSpeed;
    if (targetZoomLevel < minZoomLevel) {  // If the target zoom level is less than the min, clamp it
        targetZoomLevel = minZoomLevel;
        this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }

    if (zoomLevel > targetZoomLevel) {  // Only zoom out if not already at the min zoom level
        qreal factor = qPow(2.0, targetZoomLevel);
        setTransform(QTransform::fromScale(factor, factor));
        if (!protagonistGraphicsItems.empty()) {
            centerOn(protagonistGraphicsItems.front());
        }
        zoomLevel = targetZoomLevel; // Update the current zoom level
    }
    this->update();
}

// This function should be called after setting the background and calculating tileWidth and tileHeight.
void Game2DView::scaleEntitiesToFitView() {
    qreal sceneWidth = scene->width();
    qreal sceneHeight = scene->height();
    qreal someFactor;
    switch(backgroundNumber){
    case 1: someFactor = 25; break;
    case 2: someFactor = 25; break;
    case 3: someFactor = 25; break;
    default: someFactor = 30; break;
    }
    qreal desiredItemWidth = sceneWidth / someFactor; // 'someFactor' is a value you choose based on how many items you want to fit across the width of the view.
    qreal desiredItemHeight = sceneHeight / someFactor; // Similarly for height.

    // Calculate scale factors
    qreal scaleFactorWidth = desiredItemWidth / tileWidth;
    qreal scaleFactorHeight = desiredItemHeight / tileHeight;

    // Use the smallest scaleFactor to ensure that the aspect ratio is maintained
    qreal scaleFactor = qMin(scaleFactorWidth, scaleFactorHeight);

    // Now set the common dimensions with the scale factor applied
    EntityGraphicsItem::setCommonDimensions(tileWidth * scaleFactor, tileHeight * scaleFactor);
    EntityGraphicsItem::setTileDimensions(tileWidth, tileHeight);
}


