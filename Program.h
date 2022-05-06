#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Bug.h"
#include "FoodPellet.h"


constexpr int WIDTH = 900;
constexpr int HEIGHT = 700;

class Program {
public:
    Program();

    void update();

    void draw(sf::RenderTexture& surface);
    void drawUI(sf::RenderTexture& surface);

    void keyPressed(const sf::Keyboard::Key& key);
    void keyReleased(const sf::Keyboard::Key& key);
private:
    void reproduceBugs();
    void initializeFoodPellets();

    sf::Font _font;

    sf::Text _fpsLabel;
    sf::Text _generationLabel;

    int _generationCounter = 0;

    sf::Clock _clock;
    unsigned int _frameCounter = 0;

    std::vector<Bug> _bugs;
    std::vector<FoodPellet> _foodPellets;
    std::shared_ptr<sf::Texture> _foodPelletTexture 
        = std::shared_ptr<sf::Texture>(new sf::Texture());
    std::shared_ptr<sf::Texture> _poisonPelletTexture
        = std::shared_ptr<sf::Texture>(new sf::Texture());
};

#endif