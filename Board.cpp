#include "Board.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <ctime>

Board::Board() {
    std::srand(static_cast<unsigned int>(time(nullptr)));
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
    for (auto &entry: positionMap) {
        auto &bugs = entry.second;
        if (bugs.size() > 1) {
            // Sort by size descending
            std::sort(bugs.begin(), bugs.end(), [](Crawler* a, Crawler* b) {
                return a->getSize() > b->getSize();
            });

            // Get all bugs with max size
            int maxSize = bugs[0]->getSize();
            std::vector<Crawler*> maxSizeBugs;
            for (auto bug: bugs) {
                if (bug->getSize() == maxSize) {
                    maxSizeBugs.push_back(bug);
                } else {
                    break;
                }
            }

            // Randomly select survivor if multiple max-size bugs
            Crawler* survivor;
            if (maxSizeBugs.size() > 1) {
                int randomIndex = rand() % maxSizeBugs.size();
                survivor = maxSizeBugs[randomIndex];
            } else {
                survivor = maxSizeBugs[0];
            }

            // Calculate total size to grow
            int totalEatenSize = 0;
            for (auto bug: bugs) {
                if (bug != survivor) {
                    totalEatenSize += bug->getSize();
                }
            }

            // Apply changes
            survivor->grow(totalEatenSize);
            for (auto bug: bugs) {
                if (bug != survivor) {
                    bug->setAlive(false);
                    bug->setKillerId(survivor->getId());
                }
            }
        }
    }
}

void Board::displayLifeHistory(std::ostream& os) const {
    for (const Crawler* crawler : crawlers) {
        os << crawler->getId() << " Crawler Path: ";
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

void Board::displayAllCells() const {
    std::map<Position, std::vector<const Crawler *> > cellMap;

    // Populate the map with alive bugs' positions
    for (const Crawler *crawler: crawlers) {
        if (crawler->isAlive()) {
            Position pos = crawler->getPosition();
            cellMap[pos].push_back(crawler);
        }
    }

    // Iterate through all cells (0,0) to (9,9) in row-major order
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            Position currentPos{x, y};
            std::cout << "(" << x << "," << y << "): ";

            auto cellEntry = cellMap.find(currentPos);
            if (cellEntry == cellMap.end()) {
                std::cout << "empty";
            } else {
                // Sort bugs in the cell by ID
                std::vector<const Crawler *> bugsInCell = cellEntry->second;
                std::sort(bugsInCell.begin(), bugsInCell.end(),
                          [](const Crawler *a, const Crawler *b) { return a->getId() < b->getId(); });

                // List all bugs in the cell
                bool firstBug = true;
                for (const Crawler *bug: bugsInCell) {
                    if (!firstBug) std::cout << ", ";
                    std::cout << "Crawler " << bug->getId();
                    firstBug = false;
                }
            }
            std::cout << std::endl;
        }
    }
}

bool Board::isGameOver() const {
    int aliveCount = 0;
    for (const Crawler* crawler : crawlers) {
        if (crawler->isAlive() && ++aliveCount > 1) {
            return false;
        }
    }
    return true;
}