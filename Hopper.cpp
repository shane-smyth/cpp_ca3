#include "Hopper.h"
#include <algorithm>

Hopper::Hopper(int id, int x, int y, Direction direction, int size, int hopLength)
    : Bug(id, x, y, direction, size), hopLength(hopLength) {
    if(hopLength < 2 || hopLength > 4) {
        this->hopLength = 2;
    }
}

void Hopper::move() {
    if (!alive) return;

    if (isWayBlocked()) {
        Direction newDirection;
        do {
            newDirection = (Direction) (1 + rand() % 4);
        } while (newDirection == direction);
        direction = newDirection;
    }

    switch (direction) {
        case Direction::NORTH:
            position.y = std::max(0, position.y - hopLength);
            break;
        case Direction::EAST:
            position.x = std::min(9, position.x + hopLength);
            break;
        case Direction::SOUTH:
            position.y = std::min(9, position.y + hopLength);
            break;
        case Direction::WEST:
            position.x = std::max(0, position.x - hopLength);
            break;
    }

    path.push_back(position);
}

int Hopper::getHopLength() const {
    return hopLength;
}
