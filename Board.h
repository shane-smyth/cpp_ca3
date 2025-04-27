#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <memory>

#include "Crawler.h"

class Board {
private:
    std::vector<std::unique_ptr<Bug>> bugs;
    bool initialized = false;

    public:
    Board();
    ~Board() = default;

    void loadFromFile(const std::string& fileName);
    void displayAllBugs() const;
    void tap();

    const std::vector<std::unique_ptr<Bug>>& getBugs() const { return bugs; }

    const Bug *findBug(int id) const;

    void displayAllCells() const;

    bool isGameOver() const;

    void displayLifeHistory(std::ostream& os=std::cout) const;

    std::vector<const Bug*> getBugsAtPosition(const Position& pos) const;

    bool isInitialized() const { return initialized; }
};



#endif //BOARD_H
