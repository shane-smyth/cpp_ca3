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
int Bug::getSize() const { return size; }
bool Bug::isAlive() const { return alive; }
const std::list<Position> &Bug::getPath() const { return path; }

bool Bug::isWayBlocked() const {
    switch (direction) {
        case Direction::NORTH: return position.y == 0;
        case Direction::EAST: return position.x == 9;
        case Direction::SOUTH: return position.y == 9;
        case Direction::WEST: return position.x == 0;
        default: return false;
    }
}

void Bug::setAlive(bool alive) { this->alive = alive; }
void Bug::grow(int amount) { size += amount; }
int Bug::getKillerId() const { return killerId; }
void Bug::setKillerId(int id) { killerId = id; }