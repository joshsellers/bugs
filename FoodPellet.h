#ifndef _FOOD_PELLET_H
#define _FOOD_PELLET_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

constexpr int FOOD_PELLET_SIZE = 10;

struct FoodPellet {
    FoodPellet(int id, sf::Vector2f pos, std::shared_ptr<sf::Texture> texture, 
        int energyValue) 
    {
        this->id = id;
        this->pos = pos;
        this->energyValue = energyValue;
        this->sprite.setTexture(*texture);
        this->sprite.setPosition(pos);
        this->sprite.setOrigin(5, 5);
    }

    int id;

    sf::Vector2f pos;
    int energyValue;

    sf::Sprite sprite;

    bool hasBeenEaten = false;
};
#endif