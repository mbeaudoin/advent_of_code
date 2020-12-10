//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <numeric>
#include <functional>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
ulong solve_puzzle1(T& data, int sizePreamble)
{
    ulong retValue = 0;

    // Mark preamble
    typename T::iterator startPreamble = data.begin();
    typename T::iterator cipher = startPreamble + sizePreamble;

    while(cipher != data.end())
    {
        bool found = false;
        for(int i=0; !found && i< sizePreamble; i++)
        {
            for(int j = i+1; !found && j < sizePreamble; j++)
            {
                if(
                    startPreamble[i] <= *cipher &&
                    startPreamble[j] <= *cipher &&
                    startPreamble[i] != startPreamble[j] &&
                    startPreamble[i] + startPreamble[j] == *cipher
                )
                {
                    found = true;
                }
            }
        }

        if(!found)
        {
            //cout << "Match: "  << *cipher << endl;
            retValue = *cipher;
            break;
        }
        else
        {
            cipher++;
            startPreamble++;
        }
    }
    return *cipher;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, ulong resPuzzle1)
{
    ulong curSum = 0;
    bool found = false;
    ulong minElem;
    ulong maxElem;
    typename T::iterator startSearch = data.begin();

    int i = 1;
    while(!found)
    {
        while(curSum <= resPuzzle1)
        {
            // Not that efficient since we keep recomputing intermediary sums...
            curSum = accumulate(startSearch, startSearch + ++i, 0);

            if(curSum == resPuzzle1)
            {
                found = true;
                minElem = *std::min_element(startSearch, startSearch+i);
                maxElem = *std::max_element(startSearch, startSearch+i);
                break;
            }
        }

        // Search next stride
        startSearch++;
        curSum = 0;
        i = 1;
    }

    return minElem + maxElem;
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
    vector<ulong> data;
    myutils::read_file<ulong, std::vector<ulong> >(data, filename, true, false);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    vector<ulong> example1 = {
        35,
        20,
        15,
        25,
        47,
        40,
        62,
        55,
        65,
        95,
        102,
        117,
        150,
        182,
        127,
        219,
        299,
        277,
        309,
        576
    };

    ulong resExample1 = solve_puzzle1<vector<ulong>>(example1, 5);
    assert( resExample1 == 127 && "Error verifying puzzle #1");

    // Solve puzzle #1
    ulong resPuzzle1 = solve_puzzle1<vector<ulong>>(data, 25) ;
    std::cout << "Answer for puzzle #1: "<< resPuzzle1 << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<ulong>>(example1, resExample1) == 62 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2<vector<ulong>>(data, resPuzzle1) << std::endl;
}
