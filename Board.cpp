#include "Board.h"
#include "Crawler.h"
#include "Hopper.h"
#include "Mantis.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <ctime>

Board::Board() {
    std::srand(static_cast<unsigned int>(time(nullptr)));
}


void Board::loadFromFile(const std::string &fileName) {
    if (initialized) {
        std::cout << "Board already initialized" << std::endl;
        return;
    }

    std::ifstream fin(fileName);
    if (fin) {
        while (!fin.eof()) {
            std::string line;
            std::getline(fin, line);
            if (line.length() > 0) {
                std::stringstream ss(line);
                char type, comma;
                int id, x, y, dir, size, hopLength = 0, flyLength = 0;

                ss >> type >> comma >> id >> comma >> x >> comma >> y >> comma >> dir >> comma >> size;

                if (type == 'H') {
                    ss >> comma >> hopLength;
                }

                if (type == 'X') {
                    ss >> comma >> flyLength;
                }

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

                switch (type) {
                    case 'C':
                        bugs.emplace_back(std::make_unique<Crawler>(id, x, y, direction, size));
                        break;
                    case 'H':
                        if (hopLength < 2 || hopLength > 4) {
                            hopLength = 2;
                        }
                        bugs.emplace_back(std::make_unique<Hopper>(id, x, y, direction, size, hopLength));
                        break;
                    case 'X':
                        if (flyLength < 2 || flyLength > 4) {
                            flyLength = 2;
                        }
                        bugs.emplace_back(std::make_unique<Mantis>(id, x, y, direction, size, flyLength));
                        break;
                    default:
                        std::cerr << "Unknown bug type: " << type << std::endl;
                }
            }
        }
        fin.close();
        initialized = true;
    } else {
        std::cerr << "Could not open file " << fileName << std::endl;
    }
}


void Board::displayAllBugs() const {
    for (const auto &bug: bugs) {
        std::string dirStr;
        switch (bug->getDirection()) {
            case Direction::NORTH: dirStr = "North";
                break;
            case Direction::EAST: dirStr = "East";
                break;
            case Direction::SOUTH: dirStr = "South";
                break;
            case Direction::WEST: dirStr = "West";
                break;
        }

        std::string typeStr = bug->getType();
        std::string hopInfo = (typeStr == "Hopper")
                                  ? " " + std::to_string(static_cast<const Hopper *>(bug.get())->getHopLength())
                                  : "";
        std::string flyInfo = (typeStr == "Mantis")
                                  ? " " + std::to_string(static_cast<const Mantis *>(bug.get())->getFlyLength())
                                  : "";

        std::printf("%03d %-7s (%d,%d) %-2d %-5s%s %s\n",
                    bug->getId(),
                    typeStr.c_str(), //https://stackoverflow.com/questions/7163069/c-string-to-enum
                    bug->getPosition().x,
                    bug->getPosition().y,
                    bug->getSize(),
                    dirStr.c_str(),
                    hopInfo.c_str(),
                    bug->isAlive() ? "Alive" : "Dead"
        );
    }
}


const Bug *Board::findBug(int id) const {
    for (const auto &bug: bugs) {
        if (bug->getId() == id) {
            return bug.get();
        }
    }
    return nullptr;
}


void Board::tap() {
    // Move all alive bugs
    for (const auto &bug: bugs) {
        if (bug->isAlive()) {
            bug->move();
            std::cout << "Bug " << bug->getId() << " moved to ("
                      << bug->getPosition().x << ","
                      << bug->getPosition().y << ")\n";
        }
    }

    // Check for collisions
    std::map<Position, std::vector<Bug *> > positionMap;
    for (const auto &bug: bugs) {
        if (bug->isAlive()) {
            positionMap[bug->getPosition()].push_back(bug.get());
        }
    }

    // Resolve collisions
    for (auto &[pos, bugsAtPos]: positionMap) {
        if (bugsAtPos.size() <= 1) continue;

        // Sort by size descending
        std::sort(bugsAtPos.begin(), bugsAtPos.end(), [](Bug *a, Bug *b) {
            return a->getSize() > b->getSize();
        });

        // Get all bugs with max size
        int maxSize = bugsAtPos[0]->getSize();
        auto firstNonMax = std::find_if(bugsAtPos.begin(), bugsAtPos.end(),
                                        [maxSize](Bug *b) { return b->getSize() < maxSize; });

        // Randomly select survivor if multiple max-size bugs
        Bug *survivor;
        if (std::distance(bugsAtPos.begin(), firstNonMax) > 1) {
            survivor = bugsAtPos[rand() % std::distance(bugsAtPos.begin(), firstNonMax)];
        } else {
            survivor = bugsAtPos[0];
        }

        // Calculate total size to grow
        int totalEatenSize = 0;
        for (Bug *bug: bugsAtPos) {
            if (bug != survivor) {
                totalEatenSize += bug->getSize();
            }
        }

        // Apply changes
        survivor->grow(totalEatenSize);
        for (Bug *bug: bugsAtPos) {
            if (bug != survivor) {
                bug->setAlive(false);
                bug->setKillerId(survivor->getId());
            }
        }
    }
}


void Board::displayLifeHistory(std::ostream &os) const {
    for (const auto &bug: bugs) {
        os << bug->getId() << " " << bug->getType() << " Path: ";
        const auto &path = bug->getPath();

        bool first = true;
        for (const auto &pos: path) {
            std::cout << (first ? "" : ",") << "(" << pos.x << "," << pos.y << ")";
            first = false;
        }

        if (!bug->isAlive()) {
            int killerId = bug->getKillerId();
            std::cout << (killerId != -1 ? " Eaten by " + std::to_string(killerId) : " Dead");
        }
        std::cout << std::endl;
    }
}


void Board::displayAllCells() const {
    std::map<Position, std::vector<const Bug *> > cellMap;

    // Populate the map with alive bugs' positions
    for (const auto &bug: bugs) {
        if (bug->isAlive()) {
            Position pos = bug->getPosition();
            cellMap[pos].push_back(bug.get());
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
                std::vector<const Bug *> bugsInCell = cellEntry->second;
                std::sort(bugsInCell.begin(), bugsInCell.end(),
                          [](const Bug *a, const Bug *b) { return a->getId() < b->getId(); });

                // List all bugs in the cell
                bool firstBug = true;
                for (const auto *bug: bugsInCell) {
                    if (!firstBug) std::cout << ", ";
                    std::cout << bug->getType() << bug->getId();
                    firstBug = false;
                }
            }
            std::cout << std::endl;
        }
    }
}


bool Board::isGameOver() const {
    int aliveCount = 0;
    for (const auto &bug: bugs) {
        if (bug->isAlive() && ++aliveCount > 1) {
            return false;
        }
    }
    return true;
}


std::vector<const Bug *> Board::getBugsAtPosition(const Position &pos) const {
    std::vector<const Bug *> result;
    for (const auto &bug: bugs) {
        if (bug->isAlive() && bug->getPosition() == pos) {
            result.push_back(bug.get());
        }
    }
    return result;
}
