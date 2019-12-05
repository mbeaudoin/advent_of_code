//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"
#include <algorithm>    // std::adjacent_find

using namespace std;

// Solve puzzle #1

int solve_puzzle1(int nMin, int nMax)
{
    int counter = 0;

    while(nMin <= nMax)
    {
        string snMin = to_string(nMin);

        if (
            is_sorted(snMin.begin(), snMin.end()) &&                 // ascending order
            adjacent_find(snMin.begin(), snMin.end()) != snMin.end() // at least one adjacent pair
        )
        {
            counter++;
        }
        nMin++;
    }

    return counter;;
}

// Solve puzzle #2
int solve_puzzle2(int nMin, int nMax)
{
    int counter = 0;

    while(nMin <= nMax)
    {
        string snMin = to_string(nMin);

        if(is_sorted(snMin.begin(), snMin.end())) // Ascending order
        {
            string::iterator startBlockOf2;
            string::iterator startBlockOf345;

            bool foundBlock345 = false;

            // Remove all group of 3, 4, 5 consecutive characters
            do
            {
                foundBlock345 = false;

                // Start by finding the next group of 2 (instead of iterating over all possible digits
                startBlockOf2 = adjacent_find(snMin.begin(), snMin.end());

                if(startBlockOf2 != snMin.end())
                {
                    for (int n : {5, 4, 3})  // Remove largest block first: 5, 4 and 3
                    {
                        startBlockOf345 = search_n(snMin.begin(), snMin.end(), n, *startBlockOf2);

                        if(startBlockOf345 != snMin.end())
                        {
                            foundBlock345 = true;
                            snMin.erase(startBlockOf345, startBlockOf345 + n);
                        }
                    }
                }
            }
            while(foundBlock345);

            // Only block of 2 consecutives characters shall remain

            if (adjacent_find(snMin.begin(), snMin.end()) != snMin.end()) // at least one adjacent pair must remains
            {
                counter++;
            }
        }
        nMin++;  // next in sequence
    }

    return counter;;
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
    auto data = myutils::read_file_csv<int, std::vector<int> >(filename, false, '-');

    cout << "Range: " << data[0] << "-" << data[1] << endl;;

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(111111, 111111)   == 1    && "Error verifying puzzle #1");
    assert(solve_puzzle1(223450, 223450)   == 0    && "Error verifying puzzle #1");
    assert(solve_puzzle1(123789, 123789)   == 0    && "Error verifying puzzle #1");
    assert(solve_puzzle1(data[0], data[1]) == 1864 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data[0], data[1]) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(112233, 112233) == 1      && "Error verifying puzzle #2");
    assert(solve_puzzle2(123444, 123444) == 0      && "Error verifying puzzle #2");
    assert(solve_puzzle2(111122, 111122) == 1      && "Error verifying puzzle #2");
    assert(solve_puzzle2(222555, 222555) == 0      && "Error verifying puzzle #2");
    assert(solve_puzzle2(data[0], data[1]) == 1258 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data[0], data[1]) << std::endl;
}
