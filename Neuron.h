#ifndef _NEURON_H
#define _NEURON_H

#include <vector>

class Neuron {
public:
    Neuron(int id);

    void update();

    void connectTo(int neuronId, float weight);

    void input(float weightedInput);

    int getId();

    void resetCurrentOutput();
    float getCurrentOutput();

    bool isPeaking();

    std::vector<int>& getOutputIds();
    std::vector<float>& getWeights();

private:
    void peak();

    bool _isPeaking = false;
    float _currentOutput = 0;
    float _lastVoltage = 0;

    int _id;

    std::vector<int> _outputs;
    std::vector<float> _weights;
};

#endif

