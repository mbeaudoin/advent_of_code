//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <numeric>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    int retValue=0;
    for(auto vi = data.begin(); vi != data.end() -1; vi++)
        if(*(vi+1) > *vi)
            retValue++;

    return retValue;;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    int retValue=0;

    // lambda function sum of n consecutive vector entry
    auto sumN = [](typename T::iterator v, int n) ->long {
        return std::accumulate(v, v+n, 0);
    };

    int windowSize = 3;

    for(auto vi = data.begin(); vi != data.end()-windowSize; vi++)
    {
        if(sumN(vi, windowSize) < sumN(vi+1, windowSize))
        {
            retValue++;
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
    std::vector<int> data;
    myutils::read_file<int, std::vector<int> >(data, filename);

    const auto example = { 199, 200, 208, 210, 200, 207, 240, 269, 260, 263 };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<int>>(example) == 7 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<int>>(example) == 5 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
