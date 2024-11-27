#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QLoggingCategory::setFilterRules("pathfinder.debug=true");

    QApplication a(argc, argv);
    // Main function or similar initialization code
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
