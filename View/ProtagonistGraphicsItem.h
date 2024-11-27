#ifndef PROTAGONISTGRAPHICSITEM_H
#define PROTAGONISTGRAPHICSITEM_H

#include "EntityGraphicsItem.h"
#include "Model/ProtagonistModel.h"
#include <QString>
#include <QDebug>

class ProtagonistGraphicsItem : public EntityGraphicsItem {
public:
    ProtagonistGraphicsItem(const ProtagonistModel& protagonistModel, const QString& baseFramesDir, QGraphicsRectItem* parent = nullptr);

private:
    QString baseFramesDir;
    void loadAnimationFrames();
    void loadFramesFromDirectory(const QString& dirPath, std::vector<QPixmap>& frames);
};

#endif // PROTAGONISTGRAPHICSITEM_H
