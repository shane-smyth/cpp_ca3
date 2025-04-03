#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Crawler.h"

class Board {
    std::vector<Crawler*> crawlers;

    public:
    Board();
    ~Board();

    void loadFromFile(const std::string& fileName);
    void displayAllBugs() const;
    void tap();

    void displayLifeHistory() const;

    const std::vector<Crawler *> &getCrawlers() const;

    const Crawler *findBug(int id) const;

    void displayAllCells() const;
};



#endif //BOARD_H
