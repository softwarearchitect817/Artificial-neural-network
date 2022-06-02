/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Functions header file
Notes: n/a
Date: 2018.12.11
************************************************************************ */
#ifndef INC_8NNETWORK_FUNCTIONS_H
#define INC_8NNETWORK_FUNCTIONS_H
#include "neuron.h"         // used for user defined Layer
#include "trainingData.h"   // used for user defined TrainingData
#include "net.h"            // used for user defined Net

void createTrainingFile(string filename);
vector<double> deNormalizeWeights(vector<double> weights);
double findMin(vector<double> weights);
double findMax(vector<double> weights);
vector<double> getMinMaxWeights();
vector<double> getTrainedWeights();
vector<double> normalizeWeights(vector<double> weights);
void saveMinMaxForDeNormalization(double min,double max);
void saveTrainedWeights(vector<Layer> m_layers);
void setTestingDataOnFile(string filename);
void showVectorVals(string label, vector<double> &v);
void testData(string file);
void trainData(string file);

#endif //INC_8NNETWORK_FUNCTIONS_H
