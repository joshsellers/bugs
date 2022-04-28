#include "Program.h"

int main() {
    constexpr int WIDTH = 900;
    constexpr int HEIGHT = 700;
    constexpr int SCALE = 2;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "StudentProject");
    window.setFramerateLimit(60);

    Program program;

    sf::Event event;

    sf::RenderTexture mainSurface;
    mainSurface.create(WIDTH, HEIGHT);
    const sf::Texture& mainSurfaceTexture = mainSurface.getTexture();

    sf::Sprite mainSurfaceSprite;
    mainSurfaceSprite.setTexture(mainSurfaceTexture);

    sf::RenderTexture uiSurface;
    uiSurface.create(WIDTH, HEIGHT);
    const sf::Texture& uiSurfaceTexture = uiSurface.getTexture();

    sf::Sprite uiSurfaceSprite;
    uiSurfaceSprite.setTexture(uiSurfaceTexture);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    program.keyPressed(event.key.code);
                    break;
                case sf::Event::KeyReleased:
                    program.keyReleased(event.key.code);
                    break;
            }
        }

        program.update();

        mainSurface.clear();
        program.draw(mainSurface);
        mainSurface.display();

        uiSurface.clear(sf::Color::Transparent);
        program.drawUI(uiSurface);
        uiSurface.display();

        window.clear();
        window.draw(mainSurfaceSprite);
        window.draw(uiSurfaceSprite);
        window.display();
    }
}