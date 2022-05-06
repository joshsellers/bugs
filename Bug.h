#ifndef _BUG_H
#define _BUG_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Neuron.h"
#include "Utility.h"

enum class NeuronIndex : int {
    // Inputs
    ClosestObjectX,
    ClosestObjectY,
    ClosestObjectAvgRed,
    ClosestObjectAvgGreen,
    ClosestObjectAvgBlue,
    CollidedWithEdge,
    CurrentEnergy,
    // Outputs
    Move,
    RotateLeft,
    RotateRight
};

enum class Direction : int {
    North,
    Northeast,
    East,
    Southeast,
    South,
    Southwest,
    West,
    Northwest
};

constexpr int ROTATE_RIGHT = 1;
constexpr int ROTATE_LEFT = -1;

constexpr float DRAG = 1.025;

constexpr int BUG_SIZE = 19;

constexpr int GENES_LENGTH = BUG_SIZE * BUG_SIZE + 1;

constexpr int BASE_ENERGY = 300;

constexpr int MOVEMENT_COST = 0;
constexpr int ROTATION_COST = 0;

class Bug {
public:
    Bug(int id, float x, float y, int genes[GENES_LENGTH],
        std::vector<float> synapseGenes);

    void setup();

    void update();

    void neuronPeaked(int neuronId);

    void rotate(int rotationDirection);
    void move();

    float getX();
    float getY();
    sf::Vector2f getPos();

    int getId();

    Direction getDirection();

    sf::Sprite& getSprite();

    float getAverageRed();
    float getAverageGreen();
    float getAverageBlue();

    Neuron& getNeuron(int index);

    int* getGenes();
    std::vector<float> getSynapseGenes();

    void eat(int energyValue);
    int getEnergy();

    int getAge();

    bool isMoving();

private:
    void setUpNeurons();
    std::vector<Neuron> _neurons;

    Direction _dir = Direction::North;
    sf::Vector2f _pos;
    sf::Vector2f _velocity;
    float _speed = 0.5f;

    std::shared_ptr<sf::Texture> _texture 
        = std::shared_ptr<sf::Texture>(new sf::Texture());
    sf::Sprite _sprite;

    int _id;

    int _genes[GENES_LENGTH] = {};
    std::vector<float> _synapseGenes;

    float _avgRed;
    float _avgGreen;
    float _avgBlue;

    int _energy = BASE_ENERGY;

    int _lifeTime = 0;
};
#endif

