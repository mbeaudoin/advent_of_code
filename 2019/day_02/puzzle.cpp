//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"

#include "Intcode.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    int currentIndex = 0;
    int opCode = data[currentIndex++];

    while(opCode != 99)
    {
        int indexParam1 = data[currentIndex++];
        int indexParam2 = data[currentIndex++];
        int indexParam3 = data[currentIndex++];

        if(opCode == 1)  // 1: Addition
        {
            data[indexParam3] = data[indexParam1] + data[indexParam2];
        }
        else             // 2: multiplication
        {
            data[indexParam3] = data[indexParam1] * data[indexParam2];
        }

        opCode = data[currentIndex++];
    }

    return data[0];
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, int targetOutput)
{
    int indexNoun = 1;
    int indexVerb = 2;
    int retValue = -1;
    bool found = false;

    // Make a copy
    T origData = data;

    for(int noun=0; noun <100 && !found; noun++)
    {
        for(int verb=0; verb <100 && !found; verb++)
        {
            // Restore data
            data = origData;

            data[indexNoun] = noun;
            data[indexVerb] = verb;

            if(solve_puzzle1(data) == targetOutput)
            {
                found = true;
                retValue = 100*noun + verb;
            }
        }
    }

    return retValue;
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
    assert(solve_puzzle1<vector<int>>({1,9,10,3,2,3,11,0,99,30,40,50}) == 3500 && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({1,0,0,0,99})                    == 2    && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({2,3,0,3,99})                    == 2    && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({2,4,4,5,99,0})                  == 2    && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({1,1,1,4,99,5,6,0,99})           == 30   && "Error verifying puzzle #1");

    // Solve puzzle #1
    data[1] = 12;
    data[2] = 2;
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(data, 4576384) == 1202 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data, 19690720) << std::endl;
}
