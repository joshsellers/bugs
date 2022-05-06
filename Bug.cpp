#include "Bug.h"
#include <iostream>

Bug::Bug(int id, float x, float y, int genes[GENES_LENGTH],
    std::vector<float> synapseGenes) {
    _id = id;
    _pos.x = x;
    _pos.y = y;
    _synapseGenes = synapseGenes;

    for (size_t i = 0; i < GENES_LENGTH; i++) {
        _genes[i] = genes[i];
    }
}

void Bug::setup() {
    _texture->create(BUG_SIZE, BUG_SIZE);

    sf::Uint8* pixels = new sf::Uint8[BUG_SIZE * BUG_SIZE * 4];

    float redTotal = 0.f;
    float greenTotal = 0.f;
    float blueTotal = 0.f;

    for (int x = 0; x < BUG_SIZE; x++) {
        for (int y = 0; y < BUG_SIZE; y++) {
            int rgb = _genes[x + y * BUG_SIZE];
            if (rgb == 0) rgb = 0xFFFFFF;
            int r = (rgb >> 16) & 0xFF;
            int g = (rgb >> 8) & 0xFF;
            int b = rgb & 0xFF;

            redTotal += (float)r;
            greenTotal += (float)g;
            blueTotal += (float)b;

            if (y < 4 && (x == 9 || x == 8 || x == 10)) {
                pixels[x * 4 + y * BUG_SIZE * 4 + 0] = r;
                pixels[x * 4 + y * BUG_SIZE * 4 + 1] = g;
                pixels[x * 4 + y * BUG_SIZE * 4 + 2] = b;
                pixels[x * 4 + y * BUG_SIZE * 4 + 3] = 0xFF;
            } else if (pow(x - 9, 2) + pow(y - 9, 2) < pow(6, 2)) {
                pixels[x * 4 + y * BUG_SIZE * 4 + 0] = r;
                pixels[x * 4 + y * BUG_SIZE * 4 + 1] = g;
                pixels[x * 4 + y * BUG_SIZE * 4 + 2] = b;
                pixels[x * 4 + y * BUG_SIZE * 4 + 3] = 0xFF;
            } else {
                pixels[x * 4 + y * BUG_SIZE * 4 + 0] = 0x00;
                pixels[x * 4 + y * BUG_SIZE * 4 + 1] = 0x00;
                pixels[x * 4 + y * BUG_SIZE * 4 + 2] = 0x00;
                pixels[x * 4 + y * BUG_SIZE * 4 + 3] = 0x00;
            }
        }
    }

    // Calculate color averages and normalize between 0 and 1
    _avgRed = (redTotal / (GENES_LENGTH - 1)) / 255;
    _avgGreen = (greenTotal / (GENES_LENGTH - 1)) / 255;
    _avgBlue = (blueTotal / (GENES_LENGTH - 1)) / 255;

    _texture->update(pixels);
    delete[] pixels;
    _sprite.setTexture(*_texture);
    _sprite.setPosition(_pos);
    _sprite.setScale(1.5, 1.5);
    _sprite.setOrigin(9, 9);

    setUpNeurons();
}

void Bug::setUpNeurons() {
    // Set up input and output neurons
    for (int i = 0; i < (int) (NeuronIndex::RotateRight) + 1; i++) {
        Neuron n(i);
        _neurons.push_back(n);
    }

    // Add genetically determined neurons
    for (int i = 0; i < _genes[GENES_LENGTH - 1]; i++) {
        Neuron n(i + ((int) (NeuronIndex::RotateRight)) + 1);
        _neurons.push_back(n);
    }

    for (int i = 0; i < _synapseGenes.size(); i += 3) {
        int fromId = (int)_synapseGenes.at(i);
        int toId = (int)_synapseGenes.at(i + 1);
        float weight = _synapseGenes.at(i + 2);

        if (fromId < _neurons.size() && toId < _neurons.size()
            && fromId >= 0 && toId >= 0 && fromId != toId) 
        {
            _neurons.at(fromId).connectTo(toId, weight);
        }
    }
}

void Bug::neuronPeaked(int neuronId) {
    // Send this neuron's output to all the neurons it's connected to
    Neuron& currentNeuron = _neurons.at(neuronId);
    for (int i = 0; i < _neurons.size(); i++) {
        Neuron& outputNeuron = _neurons.at(i);
        for (int j = 0; j < currentNeuron.getOutputIds().size(); j++) {
            if (outputNeuron.getId() == currentNeuron.getOutputIds().at(j)) {
                outputNeuron.input(
                    currentNeuron.getCurrentOutput() 
                    * currentNeuron.getWeights().at(j)
                );
            }
        }
    }
    currentNeuron.resetCurrentOutput();

    // Check if this neuron is one of the output neurons
    switch ((NeuronIndex) neuronId) {
        case NeuronIndex::Move:
            _energy -= MOVEMENT_COST;
            move();
            break;
        case NeuronIndex::RotateLeft:
            _energy -= ROTATION_COST;
            rotate(ROTATE_LEFT);
            break;
        case NeuronIndex::RotateRight:
            _energy -= ROTATION_COST;
            rotate(ROTATE_RIGHT);
            break;
    }
}

void Bug::update() {
    if (_energy / 2 != 0) {
        _neurons.at((int)NeuronIndex::CurrentEnergy).input(1 / (_energy / 2));
    }

    for (auto& neuron : _neurons) {
        neuron.update();
        if (neuron.isPeaking()) {
            neuronPeaked(neuron.getId());
        }
    }

    sf::FloatRect bounds = _sprite.getGlobalBounds();
    sf::Vector2f newPos = _pos + _velocity;
    if (newPos.x - bounds.height / 2 >= 0
        && newPos.y - bounds.height / 2 >= 0
        && newPos.x + bounds.width / 2 < 900
        && newPos.y + bounds.height / 2 < 700) 
    {
        _pos += _velocity;
    } else {
        _velocity.x /= -2;
        _velocity.y /= -2;
        _neurons.at((int)NeuronIndex::CollidedWithEdge).input(1.f);
    }
    _velocity.x /= DRAG;
    _velocity.y /= DRAG;

    _sprite.setPosition(_pos);

    _energy--;
    _lifeTime++;
}


void Bug::move() {
    switch (_dir) {
        case Direction::North:
            _velocity.y -= 1 * _speed;
            break;
        case Direction::Northeast:
            _velocity.x += 0.5 * _speed;
            _velocity.y -= 0.5 * _speed;
            break;
        case Direction::East:
            _velocity.x += 1 * _speed;
            break;
        case Direction::Southeast:
            _velocity.x += 0.5 * _speed;
            _velocity.y += 0.5 * _speed;
            break;
        case Direction::South:
            _velocity.y += 1 * _speed;
            break;
        case Direction::Southwest:
            _velocity.x -= 0.5 * _speed;
            _velocity.y += 0.5 * _speed;
            break;
        case Direction::West:
            _velocity.x -= 1 * _speed;
            break;
        case Direction::Northwest:
            _velocity.x -= 0.5 * _speed;
            _velocity.y -= 0.5 * _speed;
            break;
    }
}

void Bug::rotate(int rotationDirection) {
    if (_dir == Direction::North && rotationDirection == ROTATE_LEFT)
        _dir = Direction::Northwest;
    else if (_dir == Direction::Northwest && rotationDirection == ROTATE_RIGHT)
        _dir = Direction::North;
    else
        _dir = (Direction) ((int)_dir + rotationDirection);

    _sprite.rotate(45 * rotationDirection);
}

bool Bug::isMoving() {
    return _velocity.x != 0 || _velocity.y != 0;
}

void Bug::eat(int energyValue) {
    _energy += energyValue;
}

int Bug::getAge() {
    return _lifeTime;
}

int* Bug::getGenes() {
    return _genes;
}

std::vector<float> Bug::getSynapseGenes() {
    return _synapseGenes;
}

Neuron& Bug::getNeuron(int index) {
    return _neurons.at(index);
}

int Bug::getEnergy() {
    return _energy;
}

float Bug::getX() {
    return _pos.x;
}

float Bug::getY() {
    return _pos.y;
}

sf::Vector2f Bug::getPos() {
    return _pos;
}

Direction Bug::getDirection() {
    return _dir;
}

sf::Sprite& Bug::getSprite() {
    return _sprite;
}

float Bug::getAverageRed() {
    return _avgRed;
}

float Bug::getAverageGreen() {
    return _avgGreen;
}

float Bug::getAverageBlue() {
    return _avgGreen;
}

int Bug::getId() {
    return _id;
}