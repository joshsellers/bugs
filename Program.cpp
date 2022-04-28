#include <iostream>
#include "Program.h"

Program::Program() : _bug(50, 50), _bug2(randomInt(20, 880), randomInt(20, 680)) {
    if (!_font.loadFromFile("font.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
    }

    _fpsLabel.setFont(_font);
    _fpsLabel.setCharacterSize(18);
    _fpsLabel.setFillColor(sf::Color::White);
    _fpsLabel.setString("0 fps");
    _fpsLabel.setPosition(0, 0);

    for (int i = 0; i < 10; i++) {
        Bug bug(randomInt(20, 880), randomInt(20, 680));
        bug.setup();
        _bugs.push_back(bug);
    }

    _bug.setup(); _bug2.setup();
}

void Program::update() {
    _bug.update(); _bug2.update();
    for (auto& bug : _bugs) bug.update();
}

void Program::draw(sf::RenderTexture& surface) {
    //surface.draw(_bug.getSprite()); surface.draw(_bug2.getSprite());
    for (auto& bug : _bugs) surface.draw(bug.getSprite());
}

void Program::drawUI(sf::RenderTexture& surface) {
    float fps = 1.f / _clock.restart().asSeconds();
    if (_frameCounter >= 30) {
        _fpsLabel.setString(std::to_string((int)fps) + " fps");
        _frameCounter = 0;
    }

    surface.draw(_fpsLabel);

    _frameCounter++;
}

void Program::keyPressed(const sf::Keyboard::Key& key) {
    if (key == sf::Keyboard::A) _bug.rotate(ROTATE_LEFT);
    if (key == sf::Keyboard::D) _bug.rotate(ROTATE_RIGHT);
    if (key == sf::Keyboard::W) _bug.move();
}

void Program::keyReleased(const sf::Keyboard::Key& key) {

}
