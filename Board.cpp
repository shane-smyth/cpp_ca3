#include "Board.h"
#include <fstream>
#include <iostream>
#include <sstream>

Board::Board() {
    std::srand(NULL);
}

Board::~Board() {
    for (Crawler *crawler: crawlers) {
        delete crawler;
    }
    crawlers.clear();
}

void Board::loadFromFile(const std::string &fileName) {
    std::ifstream fin(fileName);
    if (fin) {
        while (!fin.eof()) {
            std::string line;
            std::getline(fin, line);
            if (line.length() > 0) {
                std::stringstream ss(line);
                char type, comma;
                int id, x, y, dir, size;

                ss >> type >> comma >> id >> comma >> x >> comma >> y >> comma >> dir >> comma >> size;

                Direction direction;
                switch (dir) {
                    case 1: direction = Direction::NORTH;
                        break;
                    case 2: direction = Direction::EAST;
                        break;
                    case 3: direction = Direction::SOUTH;
                        break;
                    case 4: direction = Direction::WEST;
                        break;
                    default:
                        std::cerr << "Invalid direction" << std::endl;
                        continue;
                }

                crawlers.push_back(new Crawler(id, x, y, direction, size));
            }
        }
        fin.close();
    } else {
        std::cerr << "Could not open file " << fileName << std::endl;
    }
}

void Board::displayAllBugs() const {
    for (const Crawler* crawler : crawlers) {
        std::string dirStr;
        switch (crawler->getDirection()) {
            case Direction::NORTH: dirStr = "North"; break;
            case Direction::EAST:  dirStr = "East";  break;
            case Direction::SOUTH: dirStr = "South"; break;
            case Direction::WEST:  dirStr = "West";  break;
        }

        std::printf("%03d Crawler (%d,%d) %-2d %-5s %s\n",
            crawler->getId(),
            crawler->getPosition().x,
            crawler->getPosition().y,
            crawler->getSize(),
            dirStr.c_str(),
            crawler->isAlive() ? "Alive" : "Dead"
        );
    }
}

const Crawler *Board::findBug(int id) const {
    for (const Crawler* crawler : crawlers) {
        if (crawler->getId() == id) {
            return crawler;
        }
    }
    return nullptr;
}

void Board::tap() {
    // Move all alive bugs
    for (Crawler* crawler : crawlers) {
        if (crawler->isAlive()) {
            crawler->move();
        }
    }
}