#include "SFMLBoard.h"
#include <iostream>

// Board configuration
const int CELL_SIZE = 80;
const int BOARD_SIZE = 10;
const float SPRITE_PADDING = 0.8f;

SFMLBoard::SFMLBoard(Board &board)
    : board(board),
      window(sf::VideoMode(CELL_SIZE * BOARD_SIZE, CELL_SIZE * BOARD_SIZE), "Bug Chess")
{
    // Load all bug textures
    loadTextures();

    // Calculate proper scaling for each bug type
    calculateAllScales();

    // Set up Super Bug sprite
    superBugSprite.setTexture(superBugTexture);
    superBugSprite.setScale(superBugScale, superBugScale);
    centerOrigin(superBugSprite);
}

void SFMLBoard::loadTextures()
{
    // Try loading all required textures
    if (!crawlerTexture.loadFromFile("assets/crawler-spider.png")) {
        std::cerr << "Error loading crawler texture!\n";
    }
    if (!hopperTexture.loadFromFile("assets/hopper-bug.png")) {
        std::cerr << "Error loading hopper texture!\n";
    }
    if (!mantisTexture.loadFromFile("assets/mantis.png")) {
        std::cerr << "Error loading mantis texture!\n";
    }
    if (!superBugTexture.loadFromFile("assets/super-beetle-bug.png")) {
        std::cerr << "Error loading Super-Bug texture!\n";
    }
}

void SFMLBoard::calculateAllScales()
{
    // Calculate individual scales for each bug type
    crawlerScale = calculateScale(crawlerTexture);
    hopperScale = calculateScale(hopperTexture);
    mantisScale = calculateScale(mantisTexture);
    superBugScale = calculateScale(superBugTexture);
}

float SFMLBoard::calculateScale(const sf::Texture& texture)
{
    // Get texture dimensions
    sf::Vector2u size = texture.getSize();

    // Find largest dimension (width or height)
    float maxSize = static_cast<float>(std::max(size.x, size.y));

    // Calculate scale to fit in cell with padding
    return (CELL_SIZE * SPRITE_PADDING) / maxSize;
}

void SFMLBoard::centerOrigin(sf::Sprite& sprite)
{
    // Center the sprite's origin point
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width/2, bounds.height/2);
}

void SFMLBoard::run()
{
    // Main game loop
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void SFMLBoard::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                handleInput(event.key);
                break;
            default:
                break;
        }
    }
}

void SFMLBoard::render()
{
    window.clear(sf::Color::White);
    drawBoard();
    drawBugs();
    window.display();
}

void SFMLBoard::drawBoard()
{
    // Draw checkerboard pattern
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor((x + y) % 2 == 0 ? sf::Color(240, 240, 240) : sf::Color(150, 150, 150));
            window.draw(cell);
        }
    }
}

void SFMLBoard::drawBugs()
{
    // Draw regular bugs
    for (const auto& bug : board.getBugs()) {
        if (!bug->isAlive()) continue;

        sf::Sprite sprite;
        float scale = 1.0f;
        const std::string type = bug->getType();

        if (type == "Crawler") {
            sprite.setTexture(crawlerTexture);
            scale = crawlerScale;
        } else if (type == "Hopper") {
            sprite.setTexture(hopperTexture);
            scale = hopperScale;
        } else if (type == "Mantis") {
            sprite.setTexture(mantisTexture);
            scale = mantisScale;
        }

        // Position and scale the sprite
        centerOrigin(sprite);
        sprite.setScale(scale, scale);
        sprite.setPosition(
            bug->getPosition().x * CELL_SIZE + CELL_SIZE/2,
            bug->getPosition().y * CELL_SIZE + CELL_SIZE/2
        );
        window.draw(sprite);
    }

    // Draw Super Bug
    superBugSprite.setPosition(
        superBugX * CELL_SIZE + CELL_SIZE/2,
        superBugY * CELL_SIZE + CELL_SIZE/2
    );
    window.draw(superBugSprite);
}

void SFMLBoard::handleInput(const sf::Event::KeyEvent& keyEvent)
{
    // Handle one movement per key press
    switch (keyEvent.code) {
        case sf::Keyboard::Left:
            if (superBugX > 0) superBugX--;
            break;
        case sf::Keyboard::Right:
            if (superBugX < BOARD_SIZE - 1) superBugX++;
            break;
        case sf::Keyboard::Up:
            if (superBugY > 0) superBugY--;
            break;
        case sf::Keyboard::Down:
            if (superBugY < BOARD_SIZE - 1) superBugY++;
            break;
        default:
            break;
    }
    checkCollisions();
}

void SFMLBoard::checkCollisions()
{
    // Check for bug collisions
    for (const auto& bug : board.getBugs()) {
        if (bug->isAlive() &&
            bug->getPosition().x == superBugX &&
            bug->getPosition().y == superBugY) {

            handleCombat(bug);
        }
    }
}

void SFMLBoard::handleCombat(const std::unique_ptr<Bug>& bug)
{
    if (bug->getSize() < 100) {
        bug->setAlive(false);
        std::cout << "Super Bug defeated a " << bug->getType() << "!\n";
    } else {
        std::cout << "Super Bug was defeated by a " << bug->getType() << "!\n";
        window.close();
    }
}