#include "Utility.h"

int randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}