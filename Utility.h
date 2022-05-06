#ifndef _UTILITY_H
#define _UTILITY_H

#include <stdlib.h>
#include <chrono>
#include <SFML/System/Vector2.hpp>

int randomInt(int min, int max);
float randomFloat(float min, float max);
long long currentTimeMillis();

float getDistance(sf::Vector2f obj1, sf::Vector2f obj2);

#endif