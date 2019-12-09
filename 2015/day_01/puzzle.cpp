//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    return count(data.begin(), data.end(), '(') - count(data.begin(), data.end(), ')') ;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    int position = 0;
    int sum = 0;

    for(char d : data)
    {
        sum = sum + (d == '(' ? 1 : -1);
        position++;

        if(sum == -1)
            break;
    }

    // cout << "Position: " << position << endl;
    return position;
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
    auto data = myutils::read_file<string, std::vector<string> >(filename);

#if 0
    for(auto d : data[0])
        cout << d;
    cout << endl;
#endif
    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(data[0]) == 138 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data[0]) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(data[0]) == 1771 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data[0]) << std::endl;
}
