#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QRegularExpression>
#include <functional>
#include <map>
#include <QStringList>
#include "Controller/WorldController.h"
#include "Controller/ViewController.h"

class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(QObject *parent = nullptr);
    ~GameController();

    void readGameStarted(bool isStarted);
    void readGameAutoplayed();
    void readGameNumberOfPlayers(const QString &numberOfPlayers);
    void readGameDifficultyLevel(const QString &difficultyLevel);
    void decideGameParameters();

    void initializeWorld();
    void reInitializeWorld();

    void switchTo2DView();
    void switchToTextView();

    void onUpArrowPressed();
    void onDownArrowPressed();
    void onLeftArrowPressed();
    void onRightArrowPressed();

    void displayHelp() const;

    bool isGameOver();
    void setGameOver();
    WorldController* getWorldController();

public slots:
    void onViewUpdated(QWidget* currentView);
    void processCommand(const QString& command);
    void onUpdateHealthAndEnergy();
    void setWon();
    void setLost();

signals:
    void viewUpdateRequested(QWidget* currentView);
    void sendTextToGUI(const QString& text) const;
    void sendGameWon();
    void sendGameLost();
    void updateStatusDisplay(int health, float energy);
    void autoPlayed();

private:
    WorldController& worldController;
    ViewController& viewController;

    bool isGameStarted;
    bool isGamePaused;
    QString gameNumberOfPlayers;
    QString gameDifficultyLevel;
    QString gameMap;
    int gameDifficultyIdx;
    float gamePRatio;

    std::map<QString, std::function<void(const QStringList&)>> commandMap;
};

#endif // GAMECONTROLLER_H
