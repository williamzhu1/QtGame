QT       += core gui

TARGET = world
TEMPLATE = lib
CONFIG += c++20
DEFINES += WORLD_LIBRARY
DEFINES += "BUILDNUMBER=700"
SOURCES += world.cpp

HEADERS += world.h\
        world_global.h

#place where library will be put on deployment, replace this by your own operating system and map
linux-g++ {
  target.path = .
  INSTALLS += target
}

RESOURCES +=
