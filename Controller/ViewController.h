#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <memory>
#include "Controller/WorldController.h"
#include "View/Game2DView.h"
#include "View/GameTextView.h"

class ViewController : public QObject {
    Q_OBJECT

public:
    static ViewController& getInstance() {
        static ViewController instance;
        return instance;
    }

    ViewController(ViewController const&) = delete;
    void operator=(ViewController const&) = delete;
    void initializeViews();

    QWidget* getCurrentView() const; // Function to get the current view

    void switchTo2DView();
    void switchToTextView();

public slots:
    void onUpdateProtagonistPosition(int protagonistIndex);
    void updateLevel();

signals:
    void viewUpdated(QWidget* currentView); // Modified signal

private:
    ViewController(QObject *parent = nullptr);

    WorldController& worldController;
    std::shared_ptr<Game2DView> game2DView;
    std::shared_ptr<GameTextView> gameTextView;
    QWidget* currentView;

};

#endif // VIEWCONTROLLER_H
