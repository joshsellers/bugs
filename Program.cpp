#include <iostream>
#include <limits>
#include <algorithm>
#include "Program.h"

constexpr int BUGS_PER_GENERATION = 10;

Program::Program() {
    if (!_font.loadFromFile("font.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
    }

    _fpsLabel.setFont(_font);
    _fpsLabel.setCharacterSize(18);
    _fpsLabel.setFillColor(sf::Color::White);
    _fpsLabel.setString("0 fps");
    _fpsLabel.setPosition(0, 0);

    _generationLabel.setFont(_font);
    _generationLabel.setCharacterSize(18);
    _generationLabel.setFillColor(sf::Color::White);
    _generationLabel.setString("Generation 0");
    _generationLabel.setPosition(0, 20);

    // Initialize bugs
    srand(currentTimeMillis());
    for (int i = 0; i < BUGS_PER_GENERATION; i++) {
        // Genes for texture RGB values
        int genes[GENES_LENGTH] = {};
        for (size_t i = 0; i < GENES_LENGTH - 1; i++) {
            int rgb = randomInt(0x44, 0xFF);
            rgb = (rgb << 8) + randomInt(0x44, 0xFF);
            rgb = (rgb << 8) + randomInt(0x44, 0xFF);
            genes[i] = rgb;
        }

        // Gene for number of neurons
        genes[GENES_LENGTH - 1] = randomInt(1, 15);

        // Genes for connections between neurons
        std::vector<float> synapseGenes;
        int numSynapses = randomInt(0, 15);
        for (int i = 0; i < numSynapses; i++) {
            synapseGenes.push_back(randomInt(0, genes[GENES_LENGTH - 1] + (int)NeuronIndex::RotateRight));
            synapseGenes.push_back(randomInt(0, genes[GENES_LENGTH - 1] + (int)NeuronIndex::RotateRight));
            synapseGenes.push_back(randomFloat(0.f, 1.f));
        }

        Bug bug(i, randomInt(20, WIDTH - 20), randomInt(20, HEIGHT - 20),
            genes, synapseGenes);
        bug.setup();
        _bugs.push_back(bug);
    }

    // Draw FoodPellet texture
    sf::Uint8* pelletPixels = new sf::Uint8[FOOD_PELLET_SIZE * FOOD_PELLET_SIZE * 4];
    for (int y = 0; y < FOOD_PELLET_SIZE; y++) {
        for (int x = 0; x < FOOD_PELLET_SIZE; x++) {
            pelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 0] = 0x00;
            pelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 1] = 0xFF;
            pelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 2] = 0x00;
            pelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 3] = 0xFF;
        }
    }

    _foodPelletTexture->create(FOOD_PELLET_SIZE, FOOD_PELLET_SIZE);
    _foodPelletTexture->update(pelletPixels);
    delete[] pelletPixels;

    // Draw PoisonPellet texture
    sf::Uint8* poisonPelletPixels = new sf::Uint8[FOOD_PELLET_SIZE * FOOD_PELLET_SIZE * 4];
    for (int y = 0; y < FOOD_PELLET_SIZE; y++) {
        for (int x = 0; x < FOOD_PELLET_SIZE; x++) {
            poisonPelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 0] = 0xFF;
            poisonPelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 1] = 0x00;
            poisonPelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 2] = 0x00;
            poisonPelletPixels[x * 4 + y * FOOD_PELLET_SIZE * 4 + 3] = 0xFF;
        }
    }

    _poisonPelletTexture->create(FOOD_PELLET_SIZE, FOOD_PELLET_SIZE);
    _poisonPelletTexture->update(poisonPelletPixels);
    delete[] poisonPelletPixels;

    // Initialize FoodPellets
    initializeFoodPellets();
}

void Program::update() {
    int totalLiveBugs = 0;

    for (auto& bug : _bugs) {
        if (bug.getEnergy() > 0) {
            totalLiveBugs++;

            // Find bug closest to this bug
            int closestBugId = 0;
            float smallestBugDistance = std::numeric_limits<float>::max();
            for (auto& otherBug : _bugs) {
                float distance = getDistance(bug.getPos(), otherBug.getPos());
                if (bug.getId() != otherBug.getId()
                    && distance < smallestBugDistance) {
                    closestBugId = otherBug.getId();
                    smallestBugDistance = distance;
                }
            }

            // Find closest FoodPellet to this bug
            int closestPelletId = 0;
            float smallestPelletDistance = std::numeric_limits<float>::max();
            for (auto& pellet : _foodPellets) {
                if (!pellet.hasBeenEaten) {
                    float distance = getDistance(bug.getPos(), pellet.pos);
                    if (distance < smallestPelletDistance) {
                        closestPelletId = pellet.id;
                        smallestPelletDistance = distance;
                    }

                    // Check if close enough to the pellet to eat it
                    if (distance < 20 && bug.isMoving()) {
                        bug.eat(pellet.energyValue);
                        pellet.hasBeenEaten = true;
                    }
                }
            }

            // Stimulate input neurons for this bug

            // If this bug is closest to a bug, then it sees the bug
            if (smallestBugDistance < smallestPelletDistance) {
                Bug& otherBug = _bugs.at(closestBugId);
                bug.getNeuron((int)NeuronIndex::ClosestObjectX).input(
                    otherBug.getX()
                );
                bug.getNeuron((int)NeuronIndex::ClosestObjectY).input(
                    otherBug.getY()
                );
                bug.getNeuron((int)NeuronIndex::ClosestObjectAvgRed).input(
                    otherBug.getAverageRed()
                );
                bug.getNeuron((int)NeuronIndex::ClosestObjectAvgGreen).input(
                    otherBug.getAverageGreen()
                );
                bug.getNeuron((int)NeuronIndex::ClosestObjectAvgBlue).input(
                    otherBug.getAverageBlue()
                );
                // If this bug is closest to a Food/PoisonPellet, then it sees the pellet
            } else {
                FoodPellet& pellet = _foodPellets.at(closestPelletId);
                bug.getNeuron((int)NeuronIndex::ClosestObjectX).input(
                    pellet.pos.x
                );
                bug.getNeuron((int)NeuronIndex::ClosestObjectY).input(
                    pellet.pos.y
                );
                if (pellet.energyValue == 50) {
                    bug.getNeuron((int)NeuronIndex::ClosestObjectAvgGreen)
                        .input(
                            1.f
                        );
                } else {
                    bug.getNeuron((int)NeuronIndex::ClosestObjectAvgRed)
                        .input(
                            1.f
                        );
                }
            }

            // Update this bug
            bug.update();
        }
    }

    if (totalLiveBugs < 3) reproduceBugs();
}

void Program::reproduceBugs() {
    int genes1[GENES_LENGTH] = {};
    int genes2[GENES_LENGTH] = {};
    std::vector<float> synapseGenes1;
    std::vector<float> synapseGenes2;

    int longestLifeSpan = 0;
    int bug1Id = -1;
    int bug2Id = -1;
    for (auto& bug : _bugs) {
        if (bug.getAge() > longestLifeSpan) {
            longestLifeSpan = bug.getAge();
            bug1Id = bug.getId();
        }
    }

    longestLifeSpan = 0;
    for (auto& bug : _bugs) {
        if (bug.getId() != bug1Id && bug.getAge() > longestLifeSpan) {
            longestLifeSpan = bug.getAge();
            bug2Id = bug.getId();
        }
    }

    Bug& bug1 = _bugs.at(bug1Id);
    Bug& bug2 = _bugs.at(bug2Id);

    for (size_t i = 0; i < GENES_LENGTH; i++) {
        genes1[i] = bug1.getGenes()[i];
        genes2[i] = bug2.getGenes()[i];
    }
    synapseGenes1 = bug1.getSynapseGenes();
    synapseGenes2 = bug2.getSynapseGenes();

    _bugs.clear();
    srand(currentTimeMillis());
    
    constexpr bool ALLOW_MUTATIONS = true;
    constexpr int MUTATION_CHANCE = 40;
    constexpr int MUTATION_RANGE_GENES = 4;
    constexpr int ADD_SYNAPSE_CHANCE = 40;
    constexpr int REMOVE_SYNAPSE_CHANCE = 40;
    constexpr float MUTATION_RANGE_SYNAPSES = 1.f;
    for (int i = 0; i < BUGS_PER_GENERATION; i++) {
        int newGenes[GENES_LENGTH] = {};
        for (size_t j = 0; j < GENES_LENGTH; j++) {
            newGenes[j] = randomInt(0, 1) == 0 ? genes1[j] : genes2[j];
            if (ALLOW_MUTATIONS && randomInt(0, MUTATION_CHANCE) == 0) {
                newGenes[j] += randomInt(
                    -MUTATION_RANGE_GENES, MUTATION_RANGE_GENES
                );

                if (j < GENES_LENGTH - 1 
                    && (newGenes[j] <= 0 || newGenes[j] > 0xFFFFFF))
                {
                    int rgb = randomInt(0x44, 0xFF);
                    rgb = (rgb << 8) + randomInt(0x44, 0xFF);
                    rgb = (rgb << 8) + randomInt(0x44, 0xFF);
                    newGenes[j] = rgb;
                } else if (j == GENES_LENGTH - 1 && newGenes[j] < 0) {
                    newGenes[j] = randomInt(0, 10);
                }
            }
        }

        std::vector<float> newSynapseGenes;
        int synapseGenesLength = 
            std::max(synapseGenes1.size(), synapseGenes2.size());
        for (int j = 0; j < synapseGenesLength; j++) {
            if (j >= synapseGenes1.size()) {
                newSynapseGenes.push_back(synapseGenes2.at(j));
            } else if (j >= synapseGenes2.size()) {
                newSynapseGenes.push_back(synapseGenes1.at(j));
            } else {
                newSynapseGenes.push_back(
                    randomInt(0, 1) == 0
                    ? synapseGenes1.at(j) : synapseGenes2.at(j)
                );
            }
        }

        if (ALLOW_MUTATIONS) {
            // Mutate synapses
            // Not so much a mutation, really just setting the weight for
            // this synapse to a new random value
            for (int j = 2; j < newSynapseGenes.size(); j += 3) {
                if (randomInt(0, MUTATION_CHANCE) == 0) {
                    newSynapseGenes.at(j) = randomFloat(
                        0.f, MUTATION_RANGE_SYNAPSES
                    );
                }
            }

            // Mutate to "remove" a synapse (really just sets the weight to 0)
            for (int j = 2; j < newSynapseGenes.size(); j += 3) {
                if (randomInt(0, REMOVE_SYNAPSE_CHANCE) == 0) {
                    newSynapseGenes.at(j) = 0.f;
                }
            }

            // Mutate to add a synapse
            if (randomInt(0, ADD_SYNAPSE_CHANCE) == 0 
                && newGenes[GENES_LENGTH - 1] + 1 != 0) 
            {
                int numSynapses = randomInt(0, 5);
                for (int j = 0; j < numSynapses; j++) {
                    int fromId = randomInt(0, newGenes[GENES_LENGTH - 1] + (int)NeuronIndex::RotateRight);
                    int toId = randomInt(0, newGenes[GENES_LENGTH - 1] + (int)NeuronIndex::RotateRight);
                    if (fromId != toId) {
                        newSynapseGenes.push_back(fromId);
                        newSynapseGenes.push_back(toId);
                        newSynapseGenes.push_back(randomFloat(0.f, 1.f));
                    }
                }
            }
        }

        Bug bug(i, randomInt(20, 880), randomInt(20, 680), 
            newGenes, newSynapseGenes);
        bug.setup();
        _bugs.push_back(bug);
    }

    // Reset food pellets
    _foodPellets.clear();
    initializeFoodPellets();

    _generationCounter++;
    _generationLabel.setString("Generation " + std::to_string(_generationCounter));
}

void Program::initializeFoodPellets() {
    constexpr int NUM_FOOD_PELLETS = 25;
    for (int i = 0; i < NUM_FOOD_PELLETS; i++) {
        int xPos = randomInt(20, WIDTH - 20);
        int yPos = randomInt(20, HEIGHT - 20);
        FoodPellet pellet(
            i, sf::Vector2f((float)xPos, (float)yPos), _foodPelletTexture, 100
        );
        _foodPellets.push_back(pellet);
    }

    constexpr int NUM_POISON_PELLETS = 25;
    for (int i = 0; i < NUM_POISON_PELLETS; i++) {
        int xPos = randomInt(20, WIDTH - 20);
        int yPos = randomInt(20, HEIGHT - 20);
        FoodPellet pellet(
            i, sf::Vector2f((float)xPos, (float)yPos), _poisonPelletTexture, -50
        );
        _foodPellets.push_back(pellet);
    }
}

void Program::draw(sf::RenderTexture& surface) {
    for (auto& bug : _bugs) if (bug.getEnergy() > 0) {
        surface.draw(bug.getSprite());
    }

    for (auto& foodPellet : _foodPellets) {
        if (!foodPellet.hasBeenEaten) {
            surface.draw(foodPellet.sprite);
        }
    }
}

void Program::drawUI(sf::RenderTexture& surface) {
    float fps = 1.f / _clock.restart().asSeconds();
    if (_frameCounter >= 30) {
        _fpsLabel.setString(std::to_string((int)fps) + " fps");
        _frameCounter = 0;
    }

    surface.draw(_fpsLabel);
    surface.draw(_generationLabel);

    _frameCounter++;
}

void Program::keyPressed(const sf::Keyboard::Key& key) {
    if (key == sf::Keyboard::Space) {
        reproduceBugs();
    }
}

void Program::keyReleased(const sf::Keyboard::Key& key) {

}
