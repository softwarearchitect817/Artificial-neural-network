#include <utility>

/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Neuron header file
Notes: Defines class Layer
Date: 2018.12.11
************************************************************************ */
#ifndef INC_8NNETWORK_NEURON_H
#define INC_8NNETWORK_NEURON_H
#include "main.h"

struct Connection {
    double weight;
    double deltaWeight;     // change in weight
};

// Declare layer variable
class Neuron;
typedef vector<Neuron> Layer;  // a vector of an undefined layer

class Neuron {

private:

    // Declare variables
    unsigned m_myIndex;  // layer[layer_x][m_myIndex]. Called in Neuron::constructor
    double m_gradient;
    static double eta;   // [0.0...1.0] overall net training rate
    static double alpha; // [0.0...n] multiplier of last weight change [momentum]
    double m_outputVal;  // vector of output values
    vector<Connection> m_outputWeights; // creates a vector of struct "connections".  Called in Neuron::constructor.

    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(); //range : [min, max)
    double sumDOW(const Layer &nextLayer) const;

public:

    Neuron(unsigned int numOutputs, unsigned int myIndex); // called from Net::Net()

    void calcHiddenGradients(const Layer &nextLayer);
    void calcOutputGradients(double targetVals);
    void feedForward(const Layer &prevLayer);
    vector<Connection> getOutputWeights(){ return m_outputWeights; }
    double getOutputVal(void) const {return m_outputVal; }
    void setOutputWeights(vector<Connection> connections) { m_outputWeights = std::move(connections); }
    void setOutputVal(double val) { m_outputVal = val; }
    void updateInputWeights(Layer &prevLayer);

};
#endif //INC_8NNETWORK_NEURON_H
