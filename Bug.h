#ifndef BUG_H
#define BUG_H
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

    bool operator<(const Position &other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};
class Bug {
protected:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;
    int killerId;

public:
    Bug(int id, int x, int y, Direction direction, int size);
    virtual void move() = 0;
    int getId() const;
    Position getPosition() const;
    Direction getDirection() const;
    int getSize() const;
    bool isAlive() const;
    const std::list<Position>& getPath() const;
    int getKillerId() const;
    void setKillerId(int id);
    void setAlive(bool alive);
    void grow(int amount);
    bool isWayBlocked() const;
    virtual std::string getType() const = 0;
    virtual ~Bug();
};


#endif
