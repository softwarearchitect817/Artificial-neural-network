/* ************************************************************************
Name: David Boyd and Jacob Reola
Assignment: 09_nNetwork
Purpose: Main source file
Notes:
      To run program: argv == "and", "or", "xor", <filename.ext>
Date: 2018.12.11
************************************************************************ */
#include "functions.h"

int main(int argc, char **argv){

    string func,
            AND = "and",
            OR  = "or",
            XOR = "xor";
    int choice;

    // Get string argv
    if (argc != 2) { cout << "Invalid argument count!\n"; return 0;}
    else if (string(argv[1]) == "and") { func = AND; }
    else if (string(argv[1]) == "or" ) { func = OR; }
    else if (string(argv[1]) == "xor") { func = XOR; }
    else {  // Test if file exists
        std::string dataFile = argv[1];
        ifstream inFile(dataFile);
        if (!inFile) { std::cout<<"\nCan't find the file!\n\n"; }
        inFile.close();
        if (inFile.fail()) { return 0; };
        func = string(argv[1]);
    }

    cout << "\n\n"
         << "||==================||\n"
         << "||====== MENU ======||\n"
         << "||==================||\n"
         << "||     Train[1]     ||\n"
         << "||     Test [2]     ||\n"
         << "||     Both [3]     ||\n"
         << "||     Quit [0]     ||\n"
         << "||====================\n"
         << "Enter choice: ";
    cin >> choice;
    cout << endl;
    switch (choice) {
        case 1:
            cout << "\n\nTraining...\n\n";
            trainData(func);
            cout << "\n\nTraining done.\n\n";
            break;
        case 2:
            cout << "\n\nTesting...\n\n";
            testData(func);
            cout << "\n\nTesting done.\n\n";
            break;
        case 3:
            cout << "\n\nTraining...\n\n";
            trainData(func);
            cout << "\n\nTraining done.\n\n";
            cout << "\n\nTesting...\n\n";
            testData(func);
            cout << "\nTesting done.\n\n";
            break;
        default:
            cout << "\n\nExiting...\n\n";
            return 0;
    }

    cout << "\n\nExiting...\n\n";
    return 0;
}
