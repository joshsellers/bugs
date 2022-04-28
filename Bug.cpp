#include "Bug.h"
#include <iostream>

Bug::Bug(float x, float y) {
    _pos.x = x;
    _pos.y = y;
}

Bug::Bug() {

}

void Bug::setup() {
    _texture.create(SIZE, SIZE);
    sf::Uint8* pixels = new sf::Uint8[SIZE * SIZE * 4];
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            if (y < 4 && (x == 9 || x == 8 || x == 10)) {
                std::cout << "test" << std::endl;
                pixels[x * 4 + y * SIZE * 4 + 0] = 0xFF;
                pixels[x * 4 + y * SIZE * 4 + 1] = 0x00;
                pixels[x * 4 + y * SIZE * 4 + 2] = 0x00;
                pixels[x * 4 + y * SIZE * 4 + 3] = 0xFF;
            } else if (pow(x - 9, 2) + pow(y - 9, 2) < pow(6, 2)) {
                pixels[x * 4 + y * SIZE * 4 + 0] = 0x00;
                pixels[x * 4 + y * SIZE * 4 + 1] = 0x99;
                pixels[x * 4 + y * SIZE * 4 + 2] = 0x99;
                pixels[x * 4 + y * SIZE * 4 + 3] = 0xFF;
            } else {
                pixels[x * 4 + y * SIZE * 4 + 0] = 0x00;
                pixels[x * 4 + y * SIZE * 4 + 1] = 0x00;
                pixels[x * 4 + y * SIZE * 4 + 2] = 0x00;
                pixels[x * 4 + y * SIZE * 4 + 3] = 0x00;
            }
        }
    }

    _texture.update(pixels);
    delete[] pixels;
    _sprite.setTexture(_texture);
    _sprite.setPosition(_pos);
    _sprite.setScale(1.5, 1.5);
    _sprite.setOrigin(9, 9);
}

void Bug::update() {
    if (randomInt(0, 40) == 0) move();
    if (randomInt(0, 20) == 0)
        rotate(randomInt(0, 1) == 0 ? ROTATE_LEFT : ROTATE_RIGHT);

    sf::FloatRect bounds = _sprite.getGlobalBounds();
    sf::Vector2f newPos = _pos + _velocity;
    if (newPos.x - bounds.height / 2 >= 0 
        && newPos.y - bounds.height / 2 >= 0 
        && newPos.x + bounds.width / 2 < 900
        && newPos.y + bounds.height / 2 < 700) {
        _pos += _velocity;
    } else {
        _velocity.x /= -2;
        _velocity.y /= -2;
    }
    _velocity.x /= DRAG;
    _velocity.y /= DRAG;

    _sprite.setPosition(_pos);
}

void Bug::move() {
    switch (_dir) {
        case direction::north:
            _velocity.y -= 1 * _speed;
            break;
        case direction::northeast:
            _velocity.x += 0.5 * _speed;
            _velocity.y -= 0.5 * _speed;
            break;
        case direction::east:
            _velocity.x += 1 * _speed;
            break;
        case direction::southeast:
            _velocity.x += 0.5 * _speed;
            _velocity.y += 0.5 * _speed;
            break;
        case direction::south:
            _velocity.y += 1 * _speed;
            break;
        case direction::southwest:
            _velocity.x -= 0.5 * _speed;
            _velocity.y += 0.5 * _speed;
            break;
        case direction::west:
            _velocity.x -= 1 * _speed;
            break;
        case direction::northwest:
            _velocity.x -= 0.5 * _speed;
            _velocity.y -= 0.5 * _speed;
            break;
    }
}

void Bug::rotate(int rotationDirection) {
    if (_dir == direction::north && rotationDirection == ROTATE_LEFT)
        _dir = direction::northwest;
    else if (_dir == direction::northwest && rotationDirection == ROTATE_RIGHT)
        _dir = direction::north;
    else
        _dir = (direction) ((int)_dir + rotationDirection);

    _sprite.rotate(45 * rotationDirection);
}

float Bug::getX() {
    return _pos.x;
}

float Bug::getY() {
    return _pos.y;
}

direction Bug::getDirection() {
    return _dir;
}

sf::Sprite& Bug::getSprite() {
    return _sprite;
}