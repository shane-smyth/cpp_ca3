#include "Crawler.h"
#include <algorithm>

// constructor
Crawler::Crawler(int id, int x, int y, Direction direction, int size)
    : Bug( id,  x,  y,  direction,  size){

}

// getters
void Crawler::move() {
    if (!alive) return;

    // Reverse direction if path is blocked
    if (isWayBlocked()) {
        direction = reverseDirection(direction);
    }

    switch (direction) {
        case Direction::NORTH:
            position.y = std::max(0, position.y - 1);
            break;
        case Direction::EAST:
            position.x = std::min(9, position.x + 1);
            break;
        case Direction::SOUTH:
            position.y = std::min(9, position.y + 1);
            break;
        case Direction::WEST:
            position.x = std::max(0, position.x - 1);
            break;
    }

    path.push_back(position);
}
