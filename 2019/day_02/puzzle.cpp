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
constexpr int solve_puzzle1(T& data, long long pokeAtIndex)
{
    cout << endl << "Solve puzzle1:" << endl;

    for(auto d : data)
        cout << d << " : " ;
    cout << endl;

    Intcode computer(data, 0, true);

    computer.run();

    return computer.pokeMemory(pokeAtIndex);
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T& data, int targetOutput)
{
    int indexNoun = 1;
    int indexVerb = 2;
    int retValue = -1;
    bool found = false;

    for(int noun=0; noun <100 && !found; noun++)
    {
        for(int verb=0; verb <100 && !found; verb++)
        {
            data[indexNoun] = noun;
            data[indexVerb] = verb;

            Intcode computer(data, 0);

            computer.run();

            if(computer.pokeMemory(0) == targetOutput)
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
    vector<int> v;
    v = {1,9,10,3,2,3,11,0,99,30,40,50}; assert(solve_puzzle1(v, 0) == 3500 && "Error verifying puzzle #1");
    v = {1,0,0,0,99};                    assert(solve_puzzle1(v, 0) == 2    && "Error verifying puzzle #1");
    v = {2,3,0,3,99};                    assert(solve_puzzle1(v, 3) == 6    && "Error verifying puzzle #1");
    v = {2,4,4,5,99,0};                  assert(solve_puzzle1(v, 5) == 9801 && "Error verifying puzzle #1");
    v = {1,1,1,4,99,5,6,0,99};           assert(solve_puzzle1(v, 0) == 30   && "Error verifying puzzle #1");

    // Solve puzzle #1
    data[1] = 12;
    data[2] = 2;
    assert(solve_puzzle1(data, 0) == 4576384   && "Error verifying puzzle #1");
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 0) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(data, 4576384) == 1202 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data, 19690720) << std::endl;
}
