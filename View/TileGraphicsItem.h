#ifndef TILEGRAPHICSITEM_H
#define TILEGRAPHICSITEM_H

#include <QGraphicsRectItem>
#include <QDebug>
#include <QDir>
#include "EntityGraphicsItem.h"
#include "Model/TileModel.h"

class TileGraphicsItem : public EntityGraphicsItem, public TileObserver {
public:
    TileGraphicsItem(const TileModel& tileModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void nextFrame() override;

    void onTileChanged() override;
    const TileModel& getTileModel();

private:
    const TileModel& tileModel;
    QString baseFramesDir;
    QPixmap backgroundImage;

    static std::vector<QPixmap> sharedIdleFrames;
    static std::vector<QPixmap> sharedPathFoundFrames;
    static std::vector<QPixmap> sharedThunderedFrames;
    static std::vector<QPixmap> sharedPoisonedFrames;

    static bool imagesLoaded;
    static void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
    static void loadSharedImages();
};

class HPGraphicsItem : public EntityGraphicsItem {
public:
    HPGraphicsItem(const TileModel& tileModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    const TileModel& tileModel;
    QString baseFramesDir;
    QPixmap backgroundImage;

    static std::vector<QPixmap> sharedIdleFrames;

    static bool imagesLoaded;
    static void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
    static void loadSharedImages();
};

class PortalGraphicsItem : public EntityGraphicsItem{
public:
    PortalGraphicsItem(const TileModel& tileModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QString baseFramesDir;

    static std::vector<QPixmap> sharedIdleFrames;

    static bool imagesLoaded;
    static void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
    static void loadSharedImages();
};

#endif // TILEGRAPHICSITEM_H

