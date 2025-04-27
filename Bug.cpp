#include "Bug.h"
#include <iostream>

using namespace std;
Bug::Bug(int id, int x, int y, Direction direction, int size)
    : id(id), position{x, y}, direction(direction), size(size), alive(true), killerId(-1) {
    path.push_back(position);
}
Bug::~Bug() {
    cout << "Bug destructor";
}

int Bug::getId() const { return id; }
Position Bug::getPosition() const { return position; }
Direction Bug::getDirection() const { return direction; }
Direction reverseDirection(Direction dir) {
    switch (dir) {
        case Direction::NORTH: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::NORTH;
        case Direction::EAST: return Direction::WEST;
        case Direction::WEST: return Direction::EAST;
        default: return dir;
    }
}

int Bug::getSize() const { return size; }
bool Bug::isAlive() const { return alive; }
const std::list<Position> &Bug::getPath() const { return path; }

bool Bug::isWayBlocked() const {
    Position nextPos = position;
    switch (direction) {
        case Direction::NORTH: nextPos.y--; break;
        case Direction::EAST: nextPos.x++; break;
        case Direction::SOUTH: nextPos.y++; break;
        case Direction::WEST: nextPos.x--; break;
    }
    return (nextPos.x < 0 || nextPos.x >= 10 || nextPos.y < 0 || nextPos.y >= 10);
}

void Bug::setAlive(bool alive) { this->alive = alive; }
void Bug::grow(int amount) { size += amount; }
int Bug::getKillerId() const { return killerId; }
void Bug::setKillerId(int id) { killerId = id; }