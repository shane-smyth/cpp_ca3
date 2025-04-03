#include "Crawler.h"
#include <algorithm>

// constructor
Crawler::Crawler(int id, int x, int y, Direction direction, int size)
    : id(id), position{x, y}, direction(direction), size(size), alive(true), killerId(-1) {
    path.push_back(position);
}

// getters
int Crawler::getId() const { return id; }
Position Crawler::getPosition() const { return position; }
Direction Crawler::getDirection() const { return direction; }
int Crawler::getSize() const { return size; }
bool Crawler::isAlive() const { return alive; }
const std::list<Position> &Crawler::getPath() const { return path; }

bool Crawler::isWayBlocked() const {
    switch (direction) {
        case Direction::NORTH: return position.y == 0;
        case Direction::EAST: return position.x == 9;
        case Direction::SOUTH: return position.y == 9;
        case Direction::WEST: return position.x == 0;
        default: return false;
    }
}

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


void Crawler::setAlive(bool alive) { this->alive = alive; }
void Crawler::grow(int amount) { size += amount; }
int Crawler::getKillerId() const { return killerId; }
void Crawler::setKillerId(int id) { killerId = id; }
