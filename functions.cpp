/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Main source file
Notes: n/a
Date: 2018.12.11
************************************************************************ */
#include "functions.h"

//============================
//=== Create Training File ===
//============================
/*
 *  Creates training file from imported dataFile
 */
void createTrainingFile(string filename){

    // Declare variables
    vector<double> inputVals, targetVals;
    int inputLayerSize = 0;
    ifstream trainingFile;
    ofstream myWritingfile;

    // Open original training file and get data
    trainingFile.open(filename);
    while (!trainingFile.eof()){
        string line;                    //  istream& getline (istream& is, string& str, char delim);
        getline(trainingFile, line);    //           getline (cin        , line                   );

        stringstream ss(line);          // reads strings and writes data into strings; breaks up words
                                        // sets the contents of the buffer to the string argument. ie) 0 1
        double oneValue, secondValue;
        ss >> oneValue;                 // initiate xRow first value from training file
        ss >> secondValue;              // initiate xRow second value from training file
        string text = line;

        istringstream iss(text);   // used to copy a string (line from training file) into the stream.
        vector<string> results( (istream_iterator<string>(iss) ), // Istream iterators are input iterators that read
                                     (istream_iterator<string>())   ); //   successive elements from an input stream
        // If there are more than 2 read-in INPUTS from t-file on any given row
        if (results.size() >= 2) {
            inputLayerSize = static_cast<int>(results.size());
            inputVals.push_back(oneValue);
            inputVals.push_back(secondValue);
        }
        else{ // Push back read-in OUTPUTS from t-file to the targetVals vector
            targetVals.push_back(oneValue);
        }
    } // end while

    // Write to train.dat
    myWritingfile.open("train.dat");
    int ipCounter = 0;
    if (myWritingfile.is_open()){
        myWritingfile << "topology: " + to_string(inputLayerSize) + " " + to_string(H_UNITS) + " 1\n";
        for (int i = 0; i < (inputVals.size() / 2); i++){
            myWritingfile << "in: " + to_string(inputVals[ipCounter]) + " " + to_string(inputVals[(ipCounter + 1)]) + "\n"; ipCounter += 2;
            myWritingfile << "out: " + to_string(targetVals[i]) + "\n";
        }
        myWritingfile.close();
    }
    trainingFile.close();

    // Display stored data from vectors
    showVectorVals("IPs:", inputVals);
    showVectorVals("OPs:", targetVals);
}

//===========================
//=== DeNormalize Weights ===
//===========================
/*
 *  Purpose  : To achieve the faster execution of the queries through introducing redundancy.
 *  Called by: functions::testData()
 *  Calls    : functions::getMinMaxWeights()
 *  Notes    : Denormalization is a strategy used on a previously-normalized database to increase performance.
 *           : The idea behind it is to add redundant data where we think it will help us the most.
 *           : We can use extra attributes in an existing table, add new tables, or even create instances of existing tables.
 *           : The goal is to decrease the running time of select queries by making data more accessible to the queries
 *           : or by generating summarized reports in separate tables.
 *  Returns  : vector<double> deNormalizedWeights
 */
vector<double> deNormalizeWeights(vector<double> weights){
    double min, max;
    vector<double> min_max = getMinMaxWeights();
    min = min_max[0];
    max = min_max[1];
    vector<double> deNormalizedWeights;
    for (double weight : weights) {
//		x(max−min)+min
        double n = weight *(max-min)+min;
        deNormalizedWeights.push_back(n);
    }
    return deNormalizedWeights;
}

//================
//=== Find Min ===
//================
/*
 * Purpose  : Find min value in weights
 * Called by: functions::normalizeWeights
 * Calls    : none.
 * Returns  : double min
 */
double findMin(vector<double> weights){
    double min;
    min = weights[0];
    for(int i = 1;i < weights.size(); ++i) {
        // Change < to > if you want to find the smallest element
        if(min > weights[i])
            min = weights[i];
    }
    return min;
}

//================
//=== Find Max ===
//================
/*
 * Purpose  : Find max value in weights vector
 * Called by: functions::normalizeWeights
 * Calls    : none
 * Returns  : double max
 */
double findMax(vector<double> weights){
    double max;
    max = weights[0];
    for(int i = 1;i < weights.size(); ++i)
    {
        // Change < to > if you want to find the smallest element
        if(max < weights[i])
            max = weights[i];
    }
    return max;
}

//===========================
//=== Get Min Max Weights ===
//===========================
/*
 * Purpose  : Initialize min&max weights vector from min_max.txt
 * Called by: functions::deNormalizeWeights
 * Calls    : min_max.txt
 * Returns  : vector<double>
 */
vector<double> getMinMaxWeights(){

    // Declare variables
    vector<double> weights;
    string  file="min_max", DATA_FILE_FORMAT = ".txt";
    string fileName = file + DATA_FILE_FORMAT;
    ifstream testingFile;

    testingFile.open(fileName);

    // Get weights from min_max.txt file
    while (!testingFile.eof()){
        string line;
        getline(testingFile, line);
        stringstream ss(line);
        std::string text = line;

        std::istringstream iss(text);
        vector<string> results((std::istream_iterator<string>(iss)),
                               std::istream_iterator<string>());
        // initialize weights vector
        for (const auto &result : results) {
            weights.push_back(stod(result));
        }
    }

    return weights;
}

//===========================
//=== Get Trained Weights ===
//===========================
/*
 * Purpose  : Get trained weights from file
 * Called by: functions::testData
 * Calls    : weights.txt
 * Returns  : vector<double> weights
 */
vector<double> getTrainedWeights(){

    // Declare variables
    vector<double> weights;
    string file="weights", DATA_FILE_FORMAT = ".txt";
    string fileName = file + DATA_FILE_FORMAT;
    ifstream testingFile;

    // Get data from weights.txt
    testingFile.open(fileName);
    while (!testingFile.eof()){
        string line;
        getline(testingFile, line);
        stringstream ss(line);
        double oneValue, secondValue;
        ss >> oneValue;
        ss >> secondValue;
        std::string text = line;

        std::istringstream iss(text);
        vector<string> results((std::istream_iterator<string>(iss)),
                               std::istream_iterator<string>());
        // initialize weights vector
        for (const auto &result : results) {
            weights.push_back(stod(result));
        }
    }

    return weights;
}

//=========================
//=== Normalize Weights ===
//=========================
/*
 *  Purpose  : Data normalization is the process of rescaling one or more attributes to the range of 0 to 1.
 *           : This means that the largest value for each attribute is 1 and the smallest value is 0.
 *  Called by: functions::testData, savedTrainedWeights()
 *  Calls    : functions::findMin, findMax, saveMinMaxForDeNormalization
 *  Returns  : vector<double>normalizedWeights()
 *  Notes    : Normalized Data f(x) = (x-min(x))/(max(x)-min(x)),
 */
vector<double> normalizeWeights(vector<double> weights) {
    double min=findMin(weights),max=findMax(weights);
    saveMinMaxForDeNormalization(min,max);
    vector<double> normalizedWeights;
    for (double weight : weights) {
        double n = (weight-min)/(max-min);
        normalizedWeights.push_back(n);
    }
    return normalizedWeights;
}

//========================================
//=== Save Min Max For DeNormalization ===
//========================================
/*
 * Purpose  : saves min & max values to min_max.txt
 * Called by: functions::normalizeWeights
 */
void saveMinMaxForDeNormalization(double min,double max){
    ofstream myfile;
    myfile.open("min_max.txt");
    myfile << to_string(min) + " ";
    myfile << to_string(max);
    myfile.close();
}

//============================
//=== Save Trained Weights ===
//============================
/*
 * Purpose  : saves training weights to weights.txt
 * Calls    : functions::normalizedWeights()
 * Called by: functions::trainData
 */
void saveTrainedWeights(vector<Layer> m_layers){

    cout << "\nSaving trained weights...";

    // Declare variables
    vector<int> n_neurons;
    vector<int> n_weights;
    vector<double> weights;

    // Create weights vector from network
    for (int i = 0; i < m_layers.size()-1;i++){
        //n_neurons.push_back(m_layers.at(i).size());
        for (int j = 0; j < m_layers.at(static_cast<unsigned long>(i)).size(); j++){
            //n_weights.push_back(m_layers[i][j].getOutputWeights().size());
            for (int k = 0; k < m_layers[i][j].getOutputWeights().size(); k++){
                weights.push_back(m_layers[i][j].getOutputWeights()[k].weight);
            }
        }
    }

    // Assign and display weights
    weights = normalizeWeights(weights);
    showVectorVals("\n\nNormalized Weights: ", weights);

    cout << "\nSaved trained weights.\n";
    cout << "\nWriting trained weights to file...";
    ofstream myfile;
    myfile.open("weights.txt");
    for (double weight : weights) {
        myfile << to_string(weight) + ", ";
    }
    myfile.close();
    cout << "\nWrite trained weights to file done.";

}

//================================
//=== Set Testing Data On File ===
//================================
/*
 * Purpose  : Save training data file to test.dat
 * Called by: functions::testData
 */
void setTestingDataOnFile(string filename) {

    // Declare variables
    int dataSize = 100;
    vector<double> inputVals, targetVals;
    int inputLayerSize = 0;
    ifstream trainingFile;
    ofstream myWritingfile;

    // Open train.dat and test values based off train.dat values
    trainingFile.open(filename.c_str());
    while (!trainingFile.eof()){
        string line;
        getline(trainingFile, line);
        stringstream ss(line);
        double oneValue, secondValue;
        ss >> oneValue;
        ss >> secondValue;
        string text = line;

        // Same comments as creatingTrainingFile
        istringstream iss(text);
        vector<string> results((istream_iterator<string>(iss)), istream_iterator<string>());

        if (results.size() >= 2) {
            //input
            inputLayerSize = static_cast<int>(results.size());
            inputVals.push_back(oneValue);
            inputVals.push_back(secondValue);
        }
        else{
            //output
            targetVals.push_back(oneValue);
        }
    }

    // Write to test.dat file
    myWritingfile.open("test.dat");
    if (myWritingfile.is_open()){
        myWritingfile << "topology: " + std::to_string(inputLayerSize) + " " + to_string(H_UNITS) + " 1\n";
        for (int j = 0; j < dataSize;j++){
            int ipCounter = 0;
            for (int i = 0; i < (inputVals.size() / 2); i++){ // IO_Pairs
                myWritingfile << "in: " + to_string(inputVals[ipCounter]) + " " + to_string(inputVals[(ipCounter + 1)]) + "\n"; ipCounter += 2;
                myWritingfile << "out: " + to_string(targetVals[i]) + "\n";
            }
        }
        myWritingfile.close();
    }
    trainingFile.close();
}

//==========================
//=== Show Vector Values ===
//==========================
/*
 * Purpose  : shows vector's values
 * Called by: functions::createTrainingFile, saveTrainedWeights
 */
void showVectorVals(string label, vector<double> &v)
{
    cout << label << " ";
    for (double i : v) {
        cout << i << " ";
    }
    cout << endl;
}

//=================
//=== Test Data ===
//=================
/*
 * Purpose  :
 * Called by: main.cpp
 * Calls    : deNormalizeWeights
 * Returns  :
 */
void testData(string file){

    // Declare variables
    int w_counter = 0;
    vector<double> weights = getTrainedWeights();
    weights = deNormalizeWeights(weights); // rescales attributes b/t -1 & 1
    string DATA_FILE_FORMAT = ".dat";
    string f = file + DATA_FILE_FORMAT;

    // train data
    setTestingDataOnFile(f);
    TrainingData trainData("test.dat");

    // create neural network
    vector<unsigned> topology;
    trainData.getTopology(topology);
    Net myNet(topology);
    vector<Layer> m_layers = myNet.m_layers;

    // Loops through weights in the neural network
    for (int i = 0; i < m_layers.size() - 1; i++){
        for (int j = 0; j < m_layers.at(static_cast<unsigned long>(i)).size(); j++){
            for (int k = 0; k < m_layers[i][j].getOutputWeights().size(); k++){
                // initialize connections vector and assign weight values
                vector<Connection> conn = m_layers[i][j].getOutputWeights();
                conn[k].weight = weights[w_counter];
                m_layers[i][j].setOutputWeights(conn);
                w_counter++;
            }
        }
    }
    myNet.m_layers = m_layers;
    // end set weights

    vector<double> inputVals, targetVals, resultVals;
    while (!trainData.isEof())
    {
        // Get new input data and feed it forward:
        if (trainData.getNextInputs(inputVals) != topology[0])
            break;
//        showVectorVals("Inputs : ", inputVals);
        myNet.feedForward(inputVals);

        // Collect the net's actual results:
        myNet.getResults(resultVals);
//        showVectorVals("Output : ", resultVals);

        // Train the net what the outputs should have been:
        trainData.getTargetOutputs(targetVals);
//        showVectorVals("Actual : ", targetVals);
        assert(targetVals.size() == topology.back());
    }
}

//==================
//=== Train Data ===
//==================
/*
 *  Reads from inputFile -> writes to train.dat
 *  Called from main.cpp
 *  Calls   functions.cpp::createTrainingFile(),
 *          TrainingData::TrainingData(), getTopology, getNextInputs,, getTargetOutputs,
 *          Net::Net(), feedForward, getResults, backProp, getRecentAverageError
 *  Note: Uncomment left-hugging comments to display training
 */
void trainData(string file){

    // Declare variables
    string DATA_FILE_FORMAT = ".dat";
    string f =  file + DATA_FILE_FORMAT;


    createTrainingFile(f); // called from functions.cpp

    TrainingData trainDATA_Obj("train.dat"); // instantiated from TrainingData class which opens train.dat file
    vector<unsigned> topology; // Topological sorting for Directed Acyclic Graph (DAG)
                               //  is a linear ordering of vertices such that for every directed edge uv,
                               //  vertex u comes before v in the ordering.
    trainDATA_Obj.getTopology(topology); // inputs values from train.dat in topological order
    Net myNet_obj(topology);   // instantiates from Net class which creates a
                               // vector representation of our graph-network of nodes

    vector<double> inputVals, targetVals, resultVals;  // vectors to hold data

    // Train Data
    for (int i = 0; i <MAX_EPOCH; i++){
        if (trainDATA_Obj.isEof()){  // clears buffer and resets pointer to beg of file
            trainDATA_Obj.resetFilePointerToTop();  // calls TrainingData::reset...();
        }
        //
        while (!trainDATA_Obj.isEof())
        {
            // Get new input data and feed it forward:
            if (trainDATA_Obj.getNextInputs(inputVals) != topology[0]) // calls TrainingData::getNextInput(int)
                break;

            // Assign input-values to next neuron in a chain-link fashion
//            showVectorVals("Inputs :", inputVals);
            myNet_obj.feedForward(inputVals);

            // Collect the net's actual results:
//            showVectorVals("Outputs:", resultVals);
            myNet_obj.getResults(resultVals);

            // Train the net what the outputs should have been:
 //            showVectorVals("Targets:", targetVals);
            trainDATA_Obj.getTargetOutputs(targetVals);
            assert(targetVals.size() == topology.back());  // assert that vectors' sizes are equivalent

            // Back Propagate data
            myNet_obj.backProp(targetVals);

            // Report how well the training is working, average over recent avg error
//            cout << "Net average error: "
//                 << myNet_obj.getRecentAverageError() << endl;

        }
    }
    // Save weights
    saveTrainedWeights(myNet_obj.m_layers);
}