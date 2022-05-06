#include "Utility.h"

int randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

float randomFloat(float min, float max) {
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

long long currentTimeMillis() {
    auto time = std::chrono::system_clock::now();

    auto since_epoch = time.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>
        (since_epoch);

    return millis.count();
}

float getDistance(sf::Vector2f obj1, sf::Vector2f obj2) {
    return sqrt(pow(obj1.x - obj2.x, 2) + pow(obj1.y - obj2.y, 2));
}
