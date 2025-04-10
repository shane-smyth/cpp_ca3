#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include "Crawler.h"

class Board {
    std::vector<Crawler*> crawlers;

    public:
    Board();
    ~Board();

    void loadFromFile(const std::string& fileName);
    void displayAllBugs() const;
    void tap();


    const std::vector<Crawler *> &getCrawlers() const;

    const Crawler *findBug(int id) const;

    void displayAllCells() const;

    bool isGameOver() const;

    void displayLifeHistory(std::ostream& os=std::cout) const;
};



#endif //BOARD_H
