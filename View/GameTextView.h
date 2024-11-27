#ifndef GAMETEXTVIEW_H
#define GAMETEXTVIEW_H

#include <QTextEdit>
#include <QDebug>
#include <vector>
#include <memory>
#include <QFont>
#include <QFontDatabase>
#include <QRegularExpression>
#include "View/GameView.h"
#include "View/EntityTextItem.h"

class GameTextView : public QTextEdit, public GameView {
    Q_OBJECT

public:
    explicit GameTextView(QWidget* parent = nullptr) : QTextEdit(parent){
        setReadOnly(true);
        // Set the font family
        QFont font;
        font.setFamily("Trebuchet MS");
        setFont(font);
    }

    void setCurrentWorld(const WorldModel& world) override;
    void initializeView() override;
    void updateView() override;

private:
    const WorldModel* world;
    std::vector<std::unique_ptr<TileTextItem>> tileTextItems;
    std::vector<std::unique_ptr<HPTextItem>> healthpackTextItems;
    std::vector<std::unique_ptr<EnemyTextItem>> enemyTextItems;
    std::vector<std::unique_ptr<PEnemyTextItem>> penemyTextItems;
    std::vector<std::unique_ptr<XEnemyTextItem>> xenemyTextItems;
    std::vector<std::unique_ptr<ProtagonistTextItem>> protagonistTextItems;

    void displayScene(const QString& sceneString);
    QString getNonTileRepresentation(int row, int col);

    template <typename T>
    QString getRepresentationAt(const std::vector<std::unique_ptr<T>>& items, int row, int col);


};

#endif // GAMETEXTVIEW_H


/** The following is the old GameTextView with ASCII-char - plian text version
 *  #ifndef GAMETEXTVIEW_H
 *  #define GAMETEXTVIEW_H
 *  #include <QTextEdit>
 *  #include <QDebug>
 *  #include <vector>
 *  #include <memory>
 *  #include <QFont>
 *  #include <QFontDatabase>
 *  #include <QRegularExpression>
 *  #include "View/GameView.h"
 *  #include "View/EntityTextItem.h"
 *  class GameTextView : public QTextEdit, public GameView {
 *      Q_OBJECT
 *  public:
 *      GameTextView(QWidget* parent = nullptr) : QTextEdit(parent), worldController(WorldController::getInstance()) {
 *          setReadOnly(true);
 *          QFont font;
 *          font.setFamily("Trebuchet MS");
 *          setFont(font);
 *      }
 *
 *      void initializeView() override;
 *      void setBackground(int backgroundNumber) override;
 *      void updateView() override;
 *
 *  private:
 *      void displayScene(const QString& sceneString);
 *      int calculateIndex(const Entity& entity) const ;
 *      std::vector<std::unique_ptr<EntityTextItem>> entityTextItems;
 *      QString backgroundString;
 *      QString sceneString;
 *      int boarderCellWidth;
 *      int dataCellWidth;
 *      int boarderGridWidth;
 *      int dataGridWidth;
 *
 *      WorldController& worldController;
 *
 *  };
 *
 *  #endif // GAMETEXTVIEW_H
 *  */
