#include "Crawler.h"
#include <algorithm>

// constructor
Crawler::Crawler(int id, int x, int y, Direction direction, int size)
    : Bug( id,  x,  y,  direction,  size){

}

// getters
void Crawler::move() {
    if (!alive) return;

    if (isWayBlocked()) {
        Direction newDirection;
        // keep generating a new direction until it is not the current direction.
        do {
            // https://cplusplus.com/forum/beginner/283229/
            newDirection = (Direction) (1 + rand() % 4);
        } while (newDirection == direction);
        direction = newDirection;
    }

    // using the min/max to never go beyond the board
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
    // record the paths taken
    path.push_back(position);
}


