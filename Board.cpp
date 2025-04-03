#include "Board.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
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
            dirStr.c_str(), //https://stackoverflow.com/questions/7163069/c-string-to-enum
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

    // Check for collisions
    std::map<Position, std::vector<Crawler*>> positionMap;
    for (Crawler* crawler : crawlers) {
        if (crawler->isAlive()) {
            positionMap[crawler->getPosition()].push_back(crawler);
        }
    }

    // Resolve collisions
    for (auto& entry : positionMap) {
        auto& bugs = entry.second;
        if (bugs.size() > 1) {
            std::sort(bugs.begin(), bugs.end(), [](Crawler* a, Crawler* b) {
                return a->getSize() > b->getSize() || (a->getSize() == b->getSize() && a->getId() < b->getId());
            });
            Crawler* survivor = bugs[0];
            for (size_t i = 1; i < bugs.size(); ++i) {
                bugs[i]->setAlive(false);
                bugs[i]->setKillerId(survivor->getId());
            }
        }
    }
}

void Board::displayLifeHistory() const {
    for (const Crawler* crawler : crawlers) {
        std::cout << crawler->getId() << " Crawler Path: ";
        const auto& path = crawler->getPath();
        bool first = true;
        for (const auto& pos : path) {
            std::cout << (first ? "" : ",") << "(" << pos.x << "," << pos.y << ")";
            first = false;
        }
        if (!crawler->isAlive()) {
            int killerId = crawler->getKillerId();
            std::cout << (killerId != -1 ? " Eaten by " + std::to_string(killerId) : " Dead");
        }
        std::cout << std::endl;
    }
}

const std::vector<Crawler*>& Board::getCrawlers() const { return crawlers; }