#include "Board.h"
#include <iostream>
#include <string>

using namespace std;

void displayMenu() {
    cout << "\nCrawler Simulator\n";
    cout << "1. Initialise Bug Board\n";
    cout << "2. Display All Bugs\n";
    cout << "3. Find a Bug\n";
    cout << "4. Tap the board\n";
    cout << "5. Display Life History\n";
    cout << "6. Display All Cells\n";
    cout << "7. Run Simulation\n";
    cout << "0. Exit\n";
}

string directionToString(Direction dir) {
    switch (dir) {
        case Direction::NORTH: return "North";
        case Direction::EAST: return "East";
        case Direction::SOUTH: return "South";
        case Direction::WEST: return "West";
        default: return "Unknown";
    }
}

int main() {
    Board board;
    int choice;
    string fileName = "crawlers.txt";

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 0:
                cout << "Exiting...\n";
                break;
            case 1:
                board.loadFromFile(fileName);
                cout << "Board Loaded" << endl;
                break;
            case 2:
                cout << "\nAll Bugs" << endl;
                board.displayAllBugs();
                break;
            case 3: {
                cout << "Enter bug ID: " << endl;
                int searchId;
                cin >> searchId;
                const Crawler *foundBug = board.findBug(searchId);

                if (foundBug) {
                    cout << "\nBug found" << endl;
                    std::printf("%03d Crawler (%d,%d) %-2d %-5s %s\n",
                                foundBug->getId(),
                                foundBug->getPosition().x,
                                foundBug->getPosition().y,
                                foundBug->getSize(),
                                directionToString(foundBug->getDirection()).c_str(), // https://stackoverflow.com/questions/7163069/c-string-to-enum
                                foundBug->isAlive() ? "Alive" : "Dead"
                    );
                } else {
                    cout << "Bug " << searchId << " not found" << endl;
                }
                break;
            }
            case 4:
                board.tap();
                cout << "Board Tapped" << endl;
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            default:
                cout << "Invalid Choice" << endl;
                break;
        }
    } while (choice != 0);

    return 0;
}
