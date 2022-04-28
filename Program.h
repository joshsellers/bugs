#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Bug.h"

class Program {
public:
    Program();

    void update();

    void draw(sf::RenderTexture& surface);
    void drawUI(sf::RenderTexture& surface);

    void keyPressed(const sf::Keyboard::Key& key);
    void keyReleased(const sf::Keyboard::Key& key);
private:
    sf::Font _font;

    sf::Text _fpsLabel;

    sf::Clock _clock;
    unsigned int _frameCounter = 0;

    Bug _bug; 
    Bug _bug2;
    std::vector<Bug> _bugs;
};

#endif