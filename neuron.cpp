/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Neuron definitions file
Notes: Creates a vector of the network of nodes
Date: 2018.12.11
************************************************************************ */
#include "neuron.h"

// Declare variables
double Neuron::eta = LR;    // overall net learning rate //0.15
double Neuron::alpha = 0.5; // momentum, multiplier of last deltaWeight, [0.0..n] // 0.5

//=============================
//=== Calc Hidden Gradients ===
//=============================
/*
 * Purpose  : Sum of Derivatives of Weights (dow)
 * Called by: net::backProp
 * Calls    : sumDOW()
 * Notes    : Machine learning uses derivatives in optimization problems.
 *          : Optimization algorithms like gradient descent use derivatives to decide whether to increase
 *          : or decrease weights in order to maximize or minimize some objective (e.g. a model’s accuracy
 *          : or error functions). Derivatives also help us approximate nonlinear functions as linear
 *          : functions (tangent lines), which have constant slopes. With a constant slope we can decide
 *          : whether to move up or down the slope (increase or decrease our weights) to get closer to
 *          : the target value (class label).
 *          :
 *          : Gradients - A gradient is a vector that stores the partial derivatives of multivariable functions.
 *          : It helps us calculate the slope at a specific point on a curve for functions with multiple independent
 *          : variables. In order to calculate this more complex slope, we need to isolate each variable to determine
 *          : how it impacts the output on its own. To do this we iterate through each of the variables and calculate
 *          : the derivative of the function after holding all other variables constant. Each iteration produces a
 *          : partial derivative which we store in the gradient.
 * Returns  : none
 */
void Neuron::calcHiddenGradients(const Layer &nextLayer) {

    double dow = sumDOW(nextLayer);
    m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

//=============================
//=== Calc Output Gradients ===
//=============================
void Neuron::calcOutputGradients(double targetVals) {

    double delta = targetVals - m_outputVal;
    m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

//===================
//=== Constructor ===
//===================
/*
 *  Purpose  : Create a vector of Connection pointers from each Neuron
 *  Called by: Net::Net
 *  Calls    : Neuron::randomWeight()
 *  Notes    : numOutputs is the number of pointers(connectors),
 *           :   then each connector is assigned a random weight
 *  Returns  : none.
 */
Neuron::Neuron(unsigned numOutputs, unsigned myIndex) {

    // loop through number of outputs(connections) from specified neuron
    for (unsigned c = 0; c < numOutputs; ++c) {
        m_outputWeights.emplace_back();  // initializes an element in the Neuron's vector of connections
        m_outputWeights.back().weight = randomWeight();  // assigns weight to Neuron.
    }

    // Assigns index to Neuron's Connection's struct (total number of connections per Neuron)
    m_myIndex = myIndex;
}

//====================
//=== Feed Forward ===
//====================
/*
 * Purpose  : Assign prevLayer's connections to all of next layer's neurons
 * Called by: Net::feedforward
 * Calls    : Neuron::transferFunction
 * Notes    :
 * Returns  : none.
 */
void Neuron::feedForward(const Layer &prevLayer) {
    double sum = 0.0;

    // Sum the previous layer's outputs (which are our inputs)
    // Include the bias node from the previous layer.
    for (const auto &n : prevLayer) {
        sum += n.getOutputVal() *
                n.m_outputWeights[m_myIndex].weight;
    }

    // Assigns output value to next layer's neuron
    m_outputVal = Neuron::transferFunction(sum);
}

//=====================
//=== Random Weight ===
//=====================
/*
 *  Purpose  :Produces a random weight;
 *  Called by: Neuron::Neuron
 *  Returns  : double
 */
double Neuron::randomWeight() { //range : [min, max)

    // Declare variables
    double min = MIN_WEIGHT, max = MAX_WEIGHT;  // creates range of random weights
    random_device rd; // random_device is a uniformly-distributed integer random number generator
                      //  that produces non-deterministic random numbers.
    mt19937 fireTruck(rd()); // Standard Mersenne_twister_engine seeded with rd()
                             // Code mt19937 is used to generate a random number,
                             // c++ is hardware based language so to generate random number we used
                             // this specific function so that it generate number according to OS

    // Produces random floating-point values i, uniformly distributed on the interval [a, b),
    // that is, distributed according to the probability density function: P(i|a,b) = 1/(b − a)
    uniform_real_distribution<> distr(min, max);
    return distr(fireTruck);
}

//===============
//=== Sum Dow ===
//===============
/*
 * Purpose  : Sum errors at the nodes we feed
 * Called by: neuron::calcHiddenGradients
 * Calls    : none.
 * Notes    :
 * Returns  : double sum
 */
double Neuron::sumDOW(const Layer &nextLayer) const {

    double sum = 0.0;

    // Sum our contributions of the errors at the nodes we feed
    for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
    }

    return sum;
}

//==========================
//=== Transfer Functions ===
//==========================
/*
 * Purpose  : Finds the hyberbolic tangent function between 0 and 1
 * Called by: neuron::feedForward
 * Calls    :
 * Notes    : Alternative to the "Sigmoid activation function"
 * Returns  : double
 */
double Neuron::transferFunction(double x) {
    return tanh(x);
}

//====================================
//=== Transfer Function Derivative ===
//====================================
/*
 * Purpose  : Finds the derivative of the tanh
 * Called by: neuron::calcHiddenGradients, calcOutputGradients
 * Calls    : none.
 * Notes    :
 * Returns  : double
 */
double Neuron::transferFunctionDerivative(double x) {
    // tanh derivative
    return 1.0 - x * x;
}

//============================
//=== Update Input Weights ===
//============================
/*
 * Purpose  : Updates input vectors
 * Called by: net::backProp
 * Calls    :
 * Notes    :
 * Returns  :
 */
void Neuron::updateInputWeights(Layer &prevLayer) {

    // The weights to be updated are in the Connection container
    // in the nuerons in the preceeding layer
    for (auto &neuron : prevLayer) {
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight =
                // Individual input, magnified by the gradient and train rate:
                eta
                * neuron.getOutputVal()
                * m_gradient
                // Also add momentum = a fraction of the previous delta weight
                + alpha
                  * oldDeltaWeight;
        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}