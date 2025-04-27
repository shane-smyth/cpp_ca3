#include "Mantis.h"
#include <algorithm>

Mantis::Mantis(int id, int x, int y, Direction dir, int size, int flyLength)
    : Bug(id, x, y, dir, size), flyLength(flyLength) {
    if(flyLength < 2 || flyLength > 4) {
        this->flyLength = 2;
    }
}

void Mantis::move() {
    if (!alive) return;

    if (isWayBlocked()) {
        direction = reverseDirection(direction);
    }

    switch (direction) {
        case Direction::NORTH:
            position.y = std::max(0, position.y - flyLength);
            break;
        case Direction::EAST:
            position.x = std::min(9, position.x + flyLength);
            break;
        case Direction::SOUTH:
            position.y = std::min(9, position.y + flyLength);
            break;
        case Direction::WEST:
            position.x = std::max(0, position.x - flyLength);
            break;
    }

    path.push_back(position);
}

int Mantis::getFlyLength() const {
    return flyLength;
}
