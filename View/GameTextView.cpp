#include "GameTextView.h"

void GameTextView::setCurrentWorld(const WorldModel& world){
    this->world = &world;
}

void GameTextView::initializeView() {
    // Extract entities from the WorldController and add them if within bounds
    const auto& tiles = world->getTiles();
    const auto& healthPacks = world->getHealthPacks();
    const auto& enemies = world->getEnemies();
    const auto& penemies = world->getPEnemies();
    const auto& xenemies = world->getXEnemies();
    const auto& protagonists = world->getProtagonists();

    tileTextItems.clear();
    healthpackTextItems.clear();
    enemyTextItems.clear();
    penemyTextItems.clear();
    xenemyTextItems.clear();
    protagonistTextItems.clear();

    for (const auto& tile : tiles) {
        tileTextItems.push_back(std::make_unique<TileTextItem>(*tile));
    }
    for (const auto& healthPack : healthPacks) {
        healthpackTextItems.push_back(std::make_unique<HPTextItem>(*healthPack));
    }
    for (const auto& enemy : enemies) {
        enemyTextItems.push_back(std::make_unique<EnemyTextItem>(*enemy));
    }
    for (const auto& penemy : penemies) {
        penemyTextItems.push_back(std::make_unique<PEnemyTextItem>(*penemy));
    }
    for (const auto& xenemy : xenemies) {
        xenemyTextItems.push_back(std::make_unique<XEnemyTextItem>(*xenemy));
    }
    for (const auto& protagonist : protagonists) {
        protagonistTextItems.push_back(std::make_unique<ProtagonistTextItem>(*protagonist));
    }
    updateView();
}


void GameTextView::updateView() {
    int rows = world->getRows();
    int cols = world->getCols();

    QString htmlString = "<html><body><pre>"; // Start HTML document

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QString representation = getNonTileRepresentation(row, col);
            if (representation.isEmpty()) {
                // If no non-tile entities are present, use tile representation
                representation = getRepresentationAt(tileTextItems, row, col);
            }
            htmlString += representation.isEmpty() ? " " : representation;
        }
        htmlString += "\n"; // New line at the end of each row
    }

    htmlString += "</pre></body></html>"; // Close HTML document
    displayScene(htmlString);
}



void GameTextView::displayScene(const QString& sceneString) {
    this->setHtml(sceneString); // Use setHtml instead of setPlainText
    this->update();
}

template <typename T>
QString GameTextView::getRepresentationAt(const std::vector<std::unique_ptr<T>>& items, int row, int col) {
    auto it = std::find_if(items.begin(), items.end(), [row, col](const std::unique_ptr<T>& item) {
        return item && item->getPosition().xCoordinate == col && item->getPosition().yCoordinate == row;
    });

    if (it != items.end()) {
        return (*it)->getTextRepresentation();
    }

    return QString();
}

// Helper function to find the first non-tile entity representation at a given row and column
QString GameTextView::getNonTileRepresentation(int row, int col) {
    QString representation;
    if (!(representation = getRepresentationAt(enemyTextItems, row, col)).isEmpty()) return representation;
    if (!(representation = getRepresentationAt(penemyTextItems, row, col)).isEmpty()) return representation;
    if (!(representation = getRepresentationAt(xenemyTextItems, row, col)).isEmpty()) return representation;
    if (!(representation = getRepresentationAt(protagonistTextItems, row, col)).isEmpty()) return representation;
    if (!(representation = getRepresentationAt(healthpackTextItems, row, col)).isEmpty()) return representation;

    return representation; // Empty if no non-tile entities are found
}


/** The following is the old GameTextView with ASCII-char - plian text version
 *  #include "GameTextView.h"
 *  void GameTextView::initializeView() {
 *      backgroundString.clear();
 *      const WorldModel& world = worldController.getCurrentWorld();
 *      // Use 'world' as needed...
 *      setBackground(worldController.getDifficultyIdx());
 *      this->setPlainText(backgroundString);
 *      // Extract entities from the WorldController and add them if within bounds
 *      const auto& tiles = world->getTiles();
 *      const auto& healthPacks = world->getHealthPacks();
 *      const auto& enemies = world->getEnemies();
 *      const auto& penemies = world->getPEnemies();
 *      const auto& xenemies = world->getXEnemies();
 *      const auto& protagonists = world->getProtagonists();
 *      for (const auto& tile : tiles) {
 *          std::unique_ptr<TileTextItem> tileTextItem = std::make_unique<TileTextItem>(*tile);
 *          tileTextItem->setIndex(calculateIndex(*tile));
 *          entityTextItems.push_back(std::move(tileTextItem));
 *      }
 *      for (const auto& healthPack : healthPacks) {
 *          std::unique_ptr<HPTextItem> healthpackTextItem = std::make_unique<HPTextItem>(*healthPack);
 *          healthpackTextItem->setIndex(calculateIndex(*healthPack));
 *          entityTextItems.push_back(std::move(healthpackTextItem));
 *      }
 *      for (const auto& enemy : enemies) {
 *          std::unique_ptr<EnemyTextItem> enemyTextItem = std::make_unique<EnemyTextItem>(*enemy);
 *          enemyTextItem->setIndex(calculateIndex(*enemy));
 *          entityTextItems.push_back(std::move(enemyTextItem));;
 *      }
 *      for (const auto& penemy : penemies) {
 *          std::unique_ptr<PEnemyTextItem> penemyTextItem = std::make_unique<PEnemyTextItem>(*penemy);
 *          penemyTextItem->setIndex(calculateIndex(*penemy));
 *          entityTextItems.push_back(std::move(penemyTextItem));
 *      }
 *      for (const auto& xenemy : xenemies) {
 *          std::unique_ptr<XEnemyTextItem> xenemyTextItem = std::make_unique<XEnemyTextItem>(*xenemy);
 *          xenemyTextItem->setIndex(calculateIndex(*xenemy));
 *          entityTextItems.push_back(std::move(xenemyTextItem));
 *      }
 *      for (const auto& protagonist : protagonists) {
 *          std::unique_ptr<ProtagonistTextItem> protagonistTextItem = std::make_unique<ProtagonistTextItem>(*protagonist);
 *          protagonistTextItem->setIndex(calculateIndex(*protagonist));
 *          entityTextItems.push_back(std::move(protagonistTextItem));
 *      }
 *      updateView();
 *  }
 *  void GameTextView::setBackground(int backgroundNumber) {
 *      Q_UNUSED(backgroundNumber);
 *      backgroundString.clear();
 *      const WorldModel& world = worldController.getCurrentWorld();
 *      int rows = world->getRows();
 *      int cols = world->getCols();
 *      QString boarderColString = ".---+";
 *      QString dataColString = "     |";
 *      QString borderGrid = "+" + boarderColString.repeated(cols) + '\n';
 *      QString dataGrid = "|" + dataColString.repeated(cols) + '\n';
 *      boarderCellWidth = boarderColString.length();
 *      dataCellWidth = dataColString.length();
 *      boarderGridWidth = borderGrid.length();
 *      dataGridWidth = dataGrid.length();
 *      // Build the top border of the grid
 *      backgroundString += borderGrid;
 *      // Construct the ASCII representation of the grid
 *      for (int row = 0; row < rows; ++row) {
 *          backgroundString += dataGrid;
 *          backgroundString += borderGrid;
 *      }
 *  }
 *
 *  void GameTextView::updateView() {
 *      sceneString = backgroundString; // Start with the base grid
 *      // update position
 *      for (const auto& entityTextItem: entityTextItems) {
 *          if (entityTextItem) {
 *              int index = calculateIndex(entityTextItem->getEntity());
 *              // Assign the index for the text item
 *              entityTextItems.back()->setIndex(index);
 *              sceneString.replace(index, 1, entityTextItem->getTextRepresentation());
 *          }
 *      }
 *
 *      // Display the updated scene
 *      displayScene(sceneString);
 *  }
 *
 *  void GameTextView::displayScene(const QString& sceneString) {
 *      this->setPlainText(sceneString);
 *       this->update();
 *  }
 *
 *  int GameTextView::calculateIndex(const Entity& entity) const {
 *      // Calculate the index for the entity's position in the backgroundString
 *      int index = boarderGridWidth + entity.getPosition().yCoordinate * (boarderGridWidth + dataGridWidth);
 *      index += ( 1 + entity.getPosition().xCoordinate * dataCellWidth + (dataCellWidth - 1) / 2);
 *      return index;
 *  }
 *
 *  */

