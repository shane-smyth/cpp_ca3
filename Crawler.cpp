#include "Crawler.h"

// constructor
Crawler::Crawler(int id, int x, int y, Direction direction, int size)
    :id(id), position(x,y), direction(direction), size(size) {}

// getters
int Crawler::getId() const {return id;}
Position Crawler::getPosition() const {return position;}
Direction Crawler::getDirection() const {return direction;}
int Crawler::getSize() const {return size;}
bool Crawler::isAlive() const {return alive;}
const std::list<Position>& Crawler::getPath() const {return path;}

bool Crawler::isWayBlocked() const {
    switch (direction) {
        case Direction::NORTH: return position.y == 0;
        case Direction::EAST: return position.y == 9;
        case Direction::SOUTH: return position.y == 9;
        case Direction::WEST: return position.y == 0;
        default: return false;
    }
}

void Crawler::move() {
    if (!alive) return;

    if (isWayBlocked()) {
        Direction newDirection;
        do {
            // https://cplusplus.com/forum/beginner/283229/
            newDirection = (Direction) (1 + rand() % 4);
        }
        while (newDirection == direction);
        direction = newDirection;
    }

    switch (direction) {
        case Direction::NORTH: position.y--; break;
        case Direction::EAST: position.x++; break;
        case Direction::SOUTH: position.y++; break;
        case Direction::WEST: position.x--; break;
    }

    path.push_back(position);
}


void Crawler::setAlive(bool alive) {this->alive = alive;}
void Crawler::grow(int amount) {size += amount;}