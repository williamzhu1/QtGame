#ifndef GAME2DVIEW_H
#define GAME2DVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QtMath>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QWheelEvent>
#include "View/GameView.h"
#include "View/EntityGraphicsItem.h"
#include "View/TileGraphicsItem.h"
#include "View/ProtagonistGraphicsItem.h"
#include "View/EnemyGraphicsItem.h"


class Game2DView : public QGraphicsView, public GameView {
    Q_OBJECT
public:
    explicit Game2DView(QWidget* parent)
        : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
        Background1.load(":/images/world_images/worldmap.png");
        Background2.load(":/images/world_images/worldmap2.png");
        Background3.load(":/images/world_images/worldmap3.png");
        zoomSpeed = 0.001; // Example value, adjust based on testing
        zoomLevel = 1.0;
        initZoomLevel = 1.0;
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setScene(scene);
    }

    virtual ~Game2DView() = default;

    /**
     * @brief initializeView
     * @param worldController
     * the 2Dview/textview should instantiate the corresponding view/textedit,
     * create a scene, based on the given input worldController extracting the entities vector list and
     * based on which iteratively create grahpicsRectItems or entityTextItem(string) and
     * add it to the scene
     */
    void setCurrentWorld(const WorldModel& world) override;
    void initializeView() override;
    void updateView() override;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void setBackground(int backgroundNumber);
    void setBackgroundNumber(int backgroundNumber);

private:
    const WorldModel* world;
    QGraphicsScene* scene;
    QPixmap Background1;
    QPixmap Background2;
    QPixmap Background3;
    QPixmap backgroundImage;
    int backgroundNumber;
    qreal zoomSpeed; // Zoom sensitivity factor
    qreal zoomLevel;
    qreal initZoomLevel;

    qreal tileWidth;
    qreal tileHeight;

    std::vector<TileGraphicsItem*> tileGraphicsItems;
    std::vector<HPGraphicsItem*> healthpackGraphicsItems;
    std::vector<PortalGraphicsItem*> portalGraphicsItems;
    std::vector<EnemyGraphicsItem*> enemyGraphicsItems;
    std::vector<PEnemyGraphicsItem*> penemyGraphicsItems;
    std::vector<XEnemyGraphicsItem*> xenemyGraphicsItems;
    std::vector<ProtagonistGraphicsItem*> protagonistGraphicsItems;

    void scaleEntitiesToFitView();
    void zoomIn(int delta);
    void zoomOut(int delta);

};

#endif // GAME2DVIEW_H
