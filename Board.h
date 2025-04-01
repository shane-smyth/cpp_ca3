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

    const Crawler *findBug(int id) const;
};



#endif //BOARD_H
