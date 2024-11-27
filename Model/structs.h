#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <memory>

struct coordinate {
    int xCoordinate;
    int yCoordinate;

    coordinate(int x, int y) : xCoordinate(x), yCoordinate(y) {}

    bool operator<(const coordinate& other) const {
        if (xCoordinate == other.xCoordinate) {
            return yCoordinate < other.yCoordinate;
        }
        return xCoordinate < other.xCoordinate;
    }

    // Spaceship operator
    auto operator<=>(const coordinate& other) const = default; // C++20 spaceship operator

    int getXPos() const{return xCoordinate;}
    int getYPos() const{return yCoordinate;}

    void setXPos(int x) {
        xCoordinate = x;
    }

    // Setter method for yCoordinate
    void setYPos(int y) {
        yCoordinate = y;
    }

    // Method to calculate the Manhattan distance to another coordinate
    int distanceTo(const coordinate& other) const {
        return std::abs(xCoordinate - other.xCoordinate) + std::abs(yCoordinate - other.yCoordinate);
    }
};

struct node {
public:
    float f, g, h;
    bool visited;
    float value;
    coordinate position;
    node* prev;
    float getValue() const {return value;}
    int getXPos() const{return position.getXPos();}
    int getYPos() const{return position.getYPos();}
    node(float node_value, coordinate& pos)
        : f(0.0f), g(0.0f), h(0.0f), visited(false), value(node_value), position(pos), prev(nullptr) {}
};


enum state { IDLE, MOVING, ATTACK, HURT, DYING, HEAL };

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#endif
