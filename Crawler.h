#ifndef CRAWLER_H
#define CRAWLER_H


#include <string>
#include "Bug.h"


class Crawler: public Bug  {

public:
    // constructor
    Crawler(int id, int x, int y, Direction direction, int size);

    // getters


    // movement methods
    void move();


    // other methods
    std::string getType() const override { return "Crawler"; }
};


#endif //CRAWLER_H
