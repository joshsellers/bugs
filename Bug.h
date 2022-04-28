#ifndef _BUG_H
#define _BUG_H

#include <SFML/Graphics.hpp>
#include "Utility.h"


enum class direction: int {
    north,
    northeast,
    east,
    southeast,
    south,
    southwest,
    west,
    northwest
};

constexpr int ROTATE_RIGHT = 1;
constexpr int ROTATE_LEFT = -1;

constexpr float DRAG = 1.025;

constexpr int SIZE = 19;

class Bug {
public:
    Bug(float x, float y);
    Bug();

    void setup();

    void update();

    float getX();
    float getY();

    direction getDirection();

    sf::Sprite& getSprite();
    void rotate(int rotationDirection);

    void move();
private:

    direction _dir = direction::north;
    sf::Vector2f _pos;
    sf::Vector2f _velocity;
    float _speed = 4;

    sf::Texture _texture;
    sf::Sprite _sprite;
};
#endif

