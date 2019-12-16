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
long long solve_puzzle1(T data, int input, bool debug = false)
{
    if(debug)
    {
        cout << endl << "Solve puzzle1:" << endl;

        for(auto d : data)
            cout << d << " : " ;
        cout << endl;
    }

    Intcode computer(data, input, debug);

    return computer.run();
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, int input, bool debug = false)
{
    Intcode computer(data, input, debug);

    return computer.run();
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
    std::vector<long long> data = myutils::read_file_csv<long long, std::vector<long long> >(filename);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    vector<long long> v;
    v = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99}; assert(solve_puzzle1(v, 0, false) == 99 && "Error verifying puzzle #1");
    v = {1102,34915192,34915192,7,4,7,99,0};                         assert(solve_puzzle1(v, 0, false) == 1219070632396864 && "Error verifying puzzle #1");
    assert(solve_puzzle1(data, 1, false) == 3742852857 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 1, false) << std::endl;

    // --------- Puzzle #2 ---------
    assert(solve_puzzle2(data, 2, false) == 73439 && "Error verifying puzzle #2");
    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle1(data, 2) << std::endl;
}
