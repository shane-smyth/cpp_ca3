#ifndef SFMLBOARD_H
#define SFMLBOARD_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Board.h"

class SFMLBoard {
private:
    // Window and board reference
    sf::RenderWindow window;
    Board &board;

    // Textures
    sf::Texture crawlerTexture;
    sf::Texture hopperTexture;
    sf::Texture mantisTexture;
    sf::Texture superBugTexture;

    // Sprites
    sf::Sprite superBugSprite;

    // Scaling factors
    float crawlerScale;
    float hopperScale;
    float mantisScale;
    float superBugScale;

    // Super Bug position
    int superBugX = 5;
    int superBugY = 5;

    // Helper functions
    void loadTextures();

    void calculateAllScales();

    float calculateScale(const sf::Texture &texture);

    void centerOrigin(sf::Sprite &sprite);

    void handleEvents();

    void render();

    void checkCollisions();

    void handleCombat(const std::unique_ptr<Bug> &bug);

public:
    explicit SFMLBoard(Board &board);

    void run();

private:
    void drawBoard();

    void drawBugs();

    void handleInput(const sf::Event::KeyEvent& keyEvent);
};

#endif //SFMLBOARD_H
