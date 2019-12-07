//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <stack>
#include "myutils.h"

#include "Intcode.h"

using namespace std;

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T ampCtrlPgm, T phaseSettings, bool debug = false)
{
    int retValue = 0;

    if(phaseSettings.size() != 0)
    {
        // Evaluate output with current phaseSettings

        for(auto i : {0, 1, 2, 3, 4})
        {
            T ampInput = {retValue, phaseSettings[i]};
            Intcode amplifiers1(ampCtrlPgm, ampInput, debug);

            retValue = amplifiers1.run();
        }

    }
    else
    {
        // Calibrate
        if(debug)
            cout << ">>>> Full calibration:" << endl;

        int maxOutput = INT_MIN;

        vector<int> amplifierSettings = {0, 1, 2, 3, 4};

        // Each phase setting is used exactly once:  So we work with all permutations of amplifierSettings
        do
        {
            T ampInputA = {0, amplifierSettings[0]};
            Intcode ampA(ampCtrlPgm, ampInputA, debug);
            int outA = ampA.run();
            if(debug)
                cout << "Run Amp A: output: " << outA << endl;

            T ampInputB = {outA, amplifierSettings[1]};
            Intcode ampB(ampCtrlPgm, ampInputB, debug);
            int outB = ampB.run();
            if(debug)
               cout << "Run Amp B: output: " << outB << endl;

            T ampInputC = {outB, amplifierSettings[2]};
            Intcode ampC(ampCtrlPgm, ampInputC, debug);
            int outC = ampC.run();
            if(debug)
               cout << "Run Amp C: output: " << outC << endl;

            T ampInputD = {outC, amplifierSettings[3]};
            Intcode ampD(ampCtrlPgm, ampInputD, debug);
            int outD = ampD.run();
            if(debug)
               cout << "Run Amp D: output: " << outD << endl;

            T ampInputE = {outD, amplifierSettings[4]};
            Intcode ampE(ampCtrlPgm, ampInputE, debug);
            int outE = ampE.run();
            if(debug)
               cout << "Run Amp E: output: " << outE << endl;

            if(debug)
            {
                for(auto i: amplifierSettings)
                    cout << i << " : ";

                cout << " : " << outA
                    << " : " << outB
                    << " : " << outC
                    << " : " << outD
                    << " : " << outE
                    << endl;
            }
            maxOutput = std::max(maxOutput, outE);

        } while (next_permutation(amplifierSettings.begin(), amplifierSettings.end()) );

        if(debug)
            cout << "Calibration max output: " << maxOutput << endl;

        retValue = maxOutput;
    }

    return retValue;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2_singleConfig(T ampCtrlPgm, T amplifierSettings, bool debug = false)
{
    int retValue = 0;

    if(debug)
    {
        cout << "Solving puzzle2 for settinga: ";
        for(auto a : amplifierSettings)
        {
            cout << a << " : ";
        }
        cout << endl;
    }

    // Run output with amplifierSettings

    // Initialize amplifiers for first loop
    T ampInputA = {0, amplifierSettings[0]};
    Intcode ampA(ampCtrlPgm, ampInputA, debug);
    ampA.setPipeOutputMode(true);
    int outA = ampA.run();
    if(debug)
        cout << "Run Amp A: output: " << outA << endl;

    T ampInputB = {outA, amplifierSettings[1]};
    Intcode ampB(ampCtrlPgm, ampInputB, debug);
    ampB.setPipeOutputMode(true);
    int outB = ampB.run();
    if(debug)
        cout << "Run Amp B: output: " << outB << endl;

    T ampInputC = {outB, amplifierSettings[2]};
    Intcode ampC(ampCtrlPgm, ampInputC, debug);
    ampC.setPipeOutputMode(true);
    int outC = ampC.run();
    if(debug)
        cout << "Run Amp C: output: " << outC << endl;

    T ampInputD = {outC, amplifierSettings[3]};
    Intcode ampD(ampCtrlPgm, ampInputD, debug);
    ampD.setPipeOutputMode(true);
    int outD = ampD.run();
    if(debug)
        cout << "Run Amp D: output: " << outD << endl;

    T ampInputE = {outD, amplifierSettings[4]};
    Intcode ampE(ampCtrlPgm, ampInputE, debug);
    ampE.setPipeOutputMode(true);
    int outE = ampE.run();
    if(debug)
        cout << "Run Amp E: output: " << outE << endl;

    // Next, we run the feedback loop
    while(!ampE.isHalted())
    {
        ampA.setInput({outE});
        outA = ampA.run();
        if(debug)
            cout << "Run Amp A: output: " << outA << endl;

        ampB.setInput({outA});
        outB = ampB.run();
        if(debug)
            cout << "Run Amp B: output: " << outB << endl;

        ampC.setInput({outB});
        outC = ampC.run();
        if(debug)
            cout << "Run Amp C: output: " << outC << endl;

        ampD.setInput({outC});
        outD = ampD.run();
        if(debug)
            cout << "Run Amp D: output: " << outD << endl;

        ampE.setInput({outD});
        outE = ampE.run();
        if(debug)
            cout << "Run Amp E: output: " << outE << endl;
    }
    retValue = outE;

    return retValue;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2(T ampCtrlPgm, bool debug = false)
{
    // Calibrate
    if(debug)
        cout << ">>>> Full calibration:" << endl;

    int maxOutput = INT_MIN;

    vector<int> amplifierSettings = {5, 6, 7, 8, 9};

    // Each phase setting is used exactly once:  So we work with all permutations of amplifierSettings
    do
    {
        int outE = solve_puzzle2_singleConfig(ampCtrlPgm, amplifierSettings, debug);
        maxOutput = std::max(maxOutput, outE);

    } while (next_permutation(amplifierSettings.begin(), amplifierSettings.end()) );

    if(debug)
        cout << "Calibration max output: " << maxOutput << endl;

    return maxOutput;
}

int main(int argc, char *argv[])
{
    // Need a command-line parameter for the input filename
    if (argc != 2) {
        cerr << "Error: missing input" << endl;
        return EXIT_FAILURE;
    }

    string filename(argv[1]);

    // This file needs to exist
    if (! myutils::file_exists(filename))
    {
        cerr << "Error: nonexistent file: " << filename << endl;
        return EXIT_FAILURE;
    }

    // Reading the data
    auto data = myutils::read_file_csv<int, std::vector<int> >(filename);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    vector<int> pgm;
    vector<int> phaseSettings;

    // Example 1
    pgm = {3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};
    phaseSettings = {4,3,2,1,0};
    assert(solve_puzzle1(pgm, phaseSettings, false) == 43210 && "Error verifying puzzle #1");

    // Example 2
    pgm = {3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0};
    phaseSettings = {0,1,2,3,4};
    assert(solve_puzzle1(pgm, phaseSettings, false) == 54321 && "Error verifying puzzle #1");

    // Example 3
    pgm = {3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};
    phaseSettings = {1,0,4,3,2};
    assert(solve_puzzle1(pgm, phaseSettings, false) == 65210 && "Error verifying puzzle #1");

    // Solve puzzle #1
    phaseSettings.clear();
    assert(solve_puzzle1(data, phaseSettings, false) == 359142 && "Error verifying puzzle #1");
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, phaseSettings) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples

    // Example 1
    phaseSettings = {9,8,7,6,5};
    pgm = {3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
        27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};
    assert(solve_puzzle2_singleConfig(pgm, phaseSettings, false) == 139629729 && "Error verifying puzzle #2");

    // Example 2
    phaseSettings = {9,7,8,5,6};
    pgm = {3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
        -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
        53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};
    assert(solve_puzzle2_singleConfig(pgm, phaseSettings, false) == 18216 && "Error verifying puzzle #1");

    // Solve puzzle #2
    assert(solve_puzzle2(data, false) == 4374895 && "Error verifying puzzle #2");
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
