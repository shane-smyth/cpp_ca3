#ifndef CRAWLER_H
#define CRAWLER_H

#include <list>
#include <string>

enum class Direction {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

struct Position {
    int x;
    int y;

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

class Crawler {
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;

public:
    // constructor
    Crawler(int id, int x, int y, Direction direction, int size);

    // getters
    int getId() const;
    Position getPosition() const;
    Direction getDirection() const;
    int getSize() const;
    bool isAlive() const;
    const std::list<Position>& getPath() const;

    // movement methods
    void move();
    bool isWayBlocked() const;

    // other methods
    void setAlive(bool alive);
    void grow(int amount);
};


#endif //CRAWLER_H
