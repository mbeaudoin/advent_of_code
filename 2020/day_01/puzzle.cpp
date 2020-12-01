//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
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
constexpr int solve_puzzle1(T data, int sumToValue)
{
    for(auto vi = data.begin(); vi != data.end(); vi++)
        for(auto vj = next(vi, 1); vj != data.end(); vj++)
            if(*vi + *vj == sumToValue)
                return *vi * *vj;

    return -1;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, int sumToValue)
{
    int retValue=0;
    for(auto vi = data.begin(); vi != data.end(); vi++)
        for(auto vj = next(vi, 1); vj != data.end(); vj++)
            for(auto vk = next(vj, 1); vk != data.end(); vk++)
                if(*vi + *vj + *vk == sumToValue)
                    return *vi * *vj * *vk;

    return -1;
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
    auto data = myutils::read_file<int, std::vector<int> >(filename);

    const auto example = { 1721, 979, 366, 299, 675, 1456};

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<int>>(example, 2020) == 514579 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 2020) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<int>>(example, 2020) == 241861950 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data, 2020) << std::endl;
}
