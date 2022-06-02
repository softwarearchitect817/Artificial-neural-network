/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Net header file
Notes: Creates a vector of the network of nodes
Date: 2018.12.11
************************************************************************ */
#ifndef INC_8NNETWORK_NET_H
#define INC_8NNETWORK_NET_H
#include "main.h"
#include "neuron.h"     // used for Layer class

class Net{

private:

    double m_error; // multilayer avg error
    double m_recentAverageError; // multilayer prev avg error
    static double m_recentAverageSmoothingFactor; // multilayer factor for smoothing used for RMS



public:

    /*
        This vector will be of size 3 as the topology is 2 2 1,
        m_layer[0] = input  layer and will have 2 neurons,
        m_layer[1] = middle layer and will have 2 neurons,
        m_layer[2] = output layer and will have 1 neuron.
        m_layer[0][0] = input layer's 1st neuron.
        m stands for multi-layer.
     */
    vector<Layer> m_layers; // *graph-network of nodes, created from Neuron class (a vector of Neurons)

    explicit Net(const vector<unsigned> &topology); // constructor

    void backProp(const vector<double> &targetVals);
    void feedForward(const vector<double> &inputVals); // called from functions.cpp::trainData
    double getRecentAverageError() const { return m_recentAverageError; }
    void getResults(vector<double> &resultVals) const;

};
#endif //INC_8NNETWORK_NET_H
