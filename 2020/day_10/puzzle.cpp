//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <numeric>      // std::adjacent_difference
#include <algorithm>    // std::adjacent_find
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    int nbrDiff1 = 1;  // accounting for charging outlet
    int nbrDiff3 = 1;  // accounting for device builtin adapter
    std::sort(data.begin(), data.end());

    //cout << " Puzzle #1:" << endl;

    for(auto i = data.begin(); i < data.end() - 1; i++)
    {
        //cout << *i << ":" << *(i+1) << ":" << *(i+1) - *i << endl;

        nbrDiff1 += (*(i+1) - *i == 1);
        nbrDiff3 += (*(i+1) - *i == 3);
    }

    return nbrDiff1 * nbrDiff3;
}

long sumOfConsecutiveNumbers(int n)
{
    long sum=0;
    for(int i=1; i<=n; i++)
        sum += i;

    return sum;
}
// Solve puzzle #2
template <typename T>
unsigned long solve_puzzle2(T data)
{
    unsigned long nbrCombinations = 0;
    // Add the charging outlet
    data.push_back(0);

    // Add the device builtin adapter
    data.push_back(*max_element(data.begin(), data.end()) + 3);

    std::sort(data.begin(), data.end());

    // Find blocks of +1 deltas
    vector<int> diff(data.size());
    std::adjacent_difference (data.begin(), data.end(), diff.begin());

    // We can remove at most 2 out of a group of n consecutive delta1

    // Compute the length of blocks of delta1
    int blockLength = 0;
    for(auto d = diff.begin(); d < diff.end() - 1; d++)
    {
        if(*d == 1 && *(d+1) == 1)
        {
            blockLength++;
        }
        else
        {
            // We have a block of delta1
            if(blockLength > 0)
            {
                // The number of possible combinations is the sum of [1..'blockLength'] numbers
                // plus 1 (the original unmodified sequence)
                int blockCombinations = sumOfConsecutiveNumbers(blockLength) + 1;

                if(nbrCombinations == 0)
                    nbrCombinations += blockCombinations;
                else
                    nbrCombinations *= blockCombinations;

                // Reset for next block
                blockLength = 0;
            }
        }
    }
    return nbrCombinations;
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
    vector<int> data;
    myutils::read_file<int, vector<int>>(data, filename, true, false);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    vector<int>example1 = {
        16,
        10,
        15,
        5,
        1,
        11,
        7,
        19,
        6,
        12,
        4};

    vector<int>example2 = {
        28,
        33,
        18,
        42,
        31,
        14,
        46,
        20,
        48,
        47,
        24,
        23,
        49,
        45,
        19,
        38,
        39,
        11,
        1,
        32,
        25,
        35,
        8,
        17,
        7,
        9,
        4,
        2,
        34,
        10,
        3
    };

    assert(solve_puzzle1<vector<int>>(example1) == 35 && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>(example2) == 220 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<int>>(example1) == 8 && "Error verifying puzzle #2 with example1");
    assert(solve_puzzle2<vector<int>>(example2) == 19208 && "Error verifying puzzle #2 with example2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
