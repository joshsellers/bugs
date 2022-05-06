#include "Bug.h"
#include <iostream>

constexpr float TIME_STEP = 1.f;
constexpr float CAPACITANCE = 100.f;
constexpr float VOLTAGE_THRESHOLD = 1.f;

Neuron::Neuron(int id) {
    _id = id;
}

void Neuron::update() {
    float voltage = _lastVoltage + TIME_STEP * (_currentOutput / CAPACITANCE);

    if (voltage > VOLTAGE_THRESHOLD) peak();
    else if (!_isPeaking) _lastVoltage = _currentOutput;
}

void Neuron::connectTo(int neuronId, float weight) {
    int index = _outputs.size();
    _outputs.push_back(neuronId);
    _weights.push_back(weight);
}

void Neuron::input(float weightedInput) {
    _currentOutput += weightedInput;
}

void Neuron::peak() {
    _isPeaking = true;
}

int Neuron::getId() {
    return _id;
}

void Neuron::resetCurrentOutput() {
    _isPeaking = false;
    _currentOutput = 0;
    _lastVoltage = 0;
}

float Neuron::getCurrentOutput() {
    return _currentOutput;
}

bool Neuron::isPeaking() {
    return _isPeaking;
}

std::vector<int>& Neuron::getOutputIds() {
    return _outputs;
}

std::vector<float>& Neuron::getWeights() {
    return _weights;
}
