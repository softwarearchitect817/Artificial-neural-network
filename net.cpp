/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Net definitions file
Notes: Creates a vector of the network of nodes
Date: 2018.12.11
************************************************************************ */
#include "net.h"

double Net::m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over

//=================
//=== Back Prop ===
//=================
/*
 * Purpose  :
 * Called by: functions::trainData
 * Calls    :
 * Notes    :
 * Returns  :
 */
void Net::backProp(const std::vector<double> &targetVals)
{
    // Calculate overall net error (RMS [root mean squared == MVT] of output neuron errors)
    Layer &outputLayer = m_layers.back();
    m_error = 0.0;

    // Calculates output error of each trained neuron
    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta *delta;
    }

    m_error /= outputLayer.size() - 1; // get average error squared
    m_error = sqrt(m_error); // RMS (Root Mean Squared = =MVT)

    // Implement a recent average measurement:
    m_recentAverageError = // how many times it needs to back propagate to learn // higher=more work, lower num = less work
            (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
            / (m_recentAverageSmoothingFactor + 1.0);

    // Calculate output layer gradients -> vector that helps calc slope for a fns with multi variables
    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        outputLayer[n].calcOutputGradients(targetVals[n]);
    }

    // Calculate gradients on hidden layers
    for (auto layerNum = static_cast<unsigned int>(m_layers.size() - 2); layerNum > 0; --layerNum) {
        Layer &hiddenLayer = m_layers[layerNum];
        Layer &nextLayer = m_layers[layerNum + 1];

        for (auto &n : hiddenLayer) {
            n.calcHiddenGradients(nextLayer);
        }
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights
    for (auto layerNum = static_cast<unsigned int>(m_layers.size() - 1); layerNum > 0; --layerNum) {
        Layer &layer = m_layers[layerNum];
        Layer &prevLayer = m_layers[layerNum - 1];

        for (unsigned n = 0; n < layer.size() - 1; ++n) {
            layer[n].updateInputWeights(prevLayer);
        }
    }
}

//===================
//=== Constructor ===
//===================
/*
 *  Purpose  : Constructs Net object - a graph of Neurons with weighted connections
 *  Called by: functions.cpp::trainData()
 *  Calls    : Neuron::Neuron(), Net::setOutputVal(int)
 *  Note     :
 *  Returns  : none.
 */
Net::Net(const vector<unsigned> &topology) {

    // Declare variables
    auto numLayers = topology.size(); // train.dat -> (2 2 1) == numLayers = 3;

    // create vector of size ***CREATES LAYER FOR LOOP***
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {  // so if numLayers == 0 then increment to 2;
        m_layers.emplace_back();  // emplace_back() - takes a variadic list of arguments
                                  //   which forwards the args and constructs an object directly
                                  //   without a temp buffer.       // m_layers defined Net.h->Neuron.h;

        // numOutputs of layer[i] is the numInputs of layer[i+1]
        // numOutputs of last layer is 0  -> because this is the ACTIVATION NEURON***
        // initiates number of connecting nodes (in hidden layer) to specified input-node
        //   which excludes the bias
        auto numOutputs = (layerNum == topology.size() - 1) ? ( 0 ) : ( topology[layerNum + 1] );

        // We have made a new Layer, now fill it ith neurons, and
        // add a bias neuron to the layer:
        unsigned neuronNum;
        //***CREATES NEURON FOR LOOP*** (excluding bias)
        for (neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
            // create a Neuron's "node" of edges, aka, "connections" with weights
            m_layers.back().push_back(Neuron(numOutputs, neuronNum)); // calls to Neuron class
        }

        //***CREATES BIAS*** (at end of layer)
        // Force the bias node's output value to 1.0. It's the last neuron created above
        m_layers.back().back().setOutputVal(BIAS_ACTIVATION); // calls Net.h::setOutputVal(m_outputWeights=...)
    } // end LAYER for loop
}

//====================
//=== Feed Forward ===
//====================
/*
 *  Purpose  : Assigns input values into next neuron's layer after network is created
 *  Called by: functions.cpp::trainData, testData;  passes vector input-values from train.dat
 *  Calls    : Neuron::setOutputVal, Net::feedForward ***Recursive
 *  Notes    : **Assigns forward
 *  Returns  : none.
 */
void Net::feedForward(const vector<double> &inputVals)
{
    // Check the num of inputVals equal to neuronnum except bias
    assert(inputVals.size() == m_layers[0].size() - 1);  // size == IO_PAIRS

    // Assign {latch/append} the input values (ie, 0, 1) into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i){
        m_layers[0][i].setOutputVal(inputVals[i]);  // assigns input value to next layer's "connection" node
    }

    // Forward propagate
    for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum){
        // Initialize a vector of layer pointers of equivalent size
        Layer &prevLayer = m_layers[layerNum - 1];

        for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n) {
            // Recursively assign input values to next layer from previous layer through a chain-link
            m_layers[layerNum][n].feedForward(prevLayer);
        }
    }
}

//===================
//=== Get Results ===
//===================
/*
 * Purpose  : Collects results values into resultVals vector
 * Called by: functions::testData, trainData
 * Calls    : none.
 * Notes    : none.
 * Returns  : none.
 */
void Net::getResults(vector<double> &resultVals) const {
    resultVals.clear();

    for (unsigned n = 0; n < m_layers.back().size() - 1; ++n) {
        resultVals.push_back(m_layers.back()[n].getOutputVal());
    }
}