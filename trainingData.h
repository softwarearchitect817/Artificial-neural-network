/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: TrainingData header file
Notes: n/a
Date: 2018.12.11
************************************************************************ */
#ifndef INC_8NNETWORK_TRAININGDATA_H
#define INC_8NNETWORK_TRAININGDATA_H
#include "main.h"

class TrainingData {

private:

    // file object used as train.dat
    ifstream m_trainingDataFile;

public:

    explicit TrainingData(string); // opens train.dat

    // Returns the number of input values read from the file:
    unsigned getNextInputs(vector<double> &i);  // called from functions.cpp::trainData()
    unsigned getTargetOutputs(vector<double> &);

    void getTopology(vector<unsigned> &topology);
    bool isEof();
    void resetFilePointerToTop(); // clears buffer and resets, called from functions.cpp::trainData()
};
#endif //INC_8NNETWORK_TRAININGDATA_H
