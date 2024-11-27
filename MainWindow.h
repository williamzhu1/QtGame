#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QComboBox>
#include <QTabWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include "Controller/GameController.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void displayView(QWidget* view);

private:
    QWidget *centralWidget;
    QTabWidget *viewTabs;
    QWidget *graphicsTab;
    QWidget *textualTab;
    QTextEdit *graphicsMessageWidget;
    QTextEdit *textualMessageWidget;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *quitButton;
    QPushButton *autoPlayButton;
    QSlider *animationDelaySlider;
    QSlider *heuristicWeightFactorSlider;
    QLabel *heuristicWeightFactorLabel;
    QLabel *animationDelayLabel;
    QVector<QLabel*> healthLabels;
    QLabel *energyLabel;
    QLabel *energyValueLabel;
    QComboBox *playerNumberComboBox;
    QLabel *playerNumberLabel;
    QComboBox *difficultyLevelComboBox;
    QLabel *difficultyLevelLabel;

    bool isGamePaused;

    GameController* gameController;

    void setupUI();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onStartButtonClicked();
    void onPauseButtonClicked();
    void onAutoPlayButtonClicked();
    void onQuitButtonClicked();
    void onViewTabChanged(int index);
    void onViewUpdateRequested(QWidget* view);
    void onDisplayText(const QString& text);
    void onUpdateStatusDisplay(int currentHealth, float currentEnergy);
    void onGameWon();
    void onGameLost();
    void onAutoPlayed();
    void onSliderValueChanged(int value);
};

#endif // MAINWINDOW_H
