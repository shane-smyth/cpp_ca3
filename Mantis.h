//
// Created by henri on 27/04/2025.
//

#ifndef MANTIS_H
#define MANTIS_H
#include "Bug.h"


class Mantis : public Bug {
private:
    int flyLength;

public:
    Mantis(int id, int x, int y, Direction dir, int size, int flyLength);

    void move() override;

    int getFlyLength() const;

    std::string getType() const override { return "Mantis"; }
};


#endif //MANTIS_H
