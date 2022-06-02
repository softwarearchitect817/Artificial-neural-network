/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: TrainingData definitions
Notes: n/a
Date: 2018.12.11
************************************************************************ */
#include "trainingData.h"

//===================
//=== Constructor ===
//===================
/*
 *  Purpose  : Opens train.dat file by initiating m_trainingDataFile from TrainingData.h
 *  Called by: functions::testData, trainData
 */
TrainingData::TrainingData(const string filename) {
    m_trainingDataFile.open(filename);
}

//=======================
//=== Get Next Inputs ===
//=======================
/*
 * Purpose  : Creates input vector from train.dat file
 * Called by: functions::trainData, testData
 * Returns  : input vector's size
 */
unsigned TrainingData::getNextInputs(vector<double> &inputVals) {

    inputVals.clear();

    string line;

    // Get inputs from train.dat file
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label == "in:") {  // push back input values only based off label: "in" in train.dat
        double oneValue;
        while (ss >> oneValue) {
            inputVals.push_back(oneValue);
        }
    }

    // return input vectors size
    return static_cast<unsigned int>(inputVals.size());
}

//==========================
//=== Get Target Outputs ===
//==========================
/*
 * Purpose  : Creates a vector that holds next layer's output values
 * Called by: functions::testData, trainData
 * Returns  : vector's size
 */
unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals) {

    targetOutputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label == "out:") {
        double oneValue;
        while (ss >> oneValue) {
            targetOutputVals.push_back(oneValue);
        }
    }

    return static_cast<unsigned int>(targetOutputVals.size());
}

//====================
//=== Get Topology ===
//====================
/*
 *  Purpose  : Creates topological-patterned vector from train.dat's values
 *  Called by: functions::testData, trainData
 *  Calls    : train.dat
 */
void TrainingData::getTopology(vector<unsigned> &topology) {

    // Declare variables
    string line;
    string label;

    // Read in lines then separate contents by ws into "label"
    getline(m_trainingDataFile, line);
    stringstream ss(line);
    ss >> label;
    // if eof
    if (this->isEof()) {
        abort();
    }

    // Push back train.dat -> row -> values
    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }
}

//==============
//=== is_EOF ===
//==============
/*
 * Purpose: check if pointer is at end of file
 */
bool TrainingData::isEof() {
    return m_trainingDataFile.eof();
}

//=================================
//=== Reset File Pointer To Top ===
//=================================
/*
 *  Purpose  : Clears buffer and resets pointer (cursor) to beg of file
 *  Called by: functions::trainData
 */
void TrainingData::resetFilePointerToTop(){
    m_trainingDataFile.clear();
    m_trainingDataFile.seekg(0);  // End the buffer with a null terminating character
}