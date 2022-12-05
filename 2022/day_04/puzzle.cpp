//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <set>
#include <numeric>
#include <algorithm>
#include <sstream>      // std::stringstream

#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data)
{
    long nbrFullOverlaps = 0L;

    for(auto val: data)
    {
        // cout << "Pair: " << val ;

        std::stringstream ss(val);

        int p1Min, p1Max;
        int p2Min, p2Max;
        char sep;

        // Extract ranges
        ss >> p1Min >> sep >> p1Max >> sep >> p2Min >> sep >> p2Max;

        auto R2OverlapR1 = [](auto r1Min, auto r1Max, auto r2Min, auto r2Max)
            {
                return
                    r1Min >= r2Min &&
                    r1Min <= r2Max &&
                    r1Max >= r2Min &&
                    r1Max <= r2Max;

            };

        // Detect full overlaps
        if (
            R2OverlapR1 (p1Min, p1Max, p2Min, p2Max)  ||
            R2OverlapR1 (p2Min, p2Max, p1Min, p1Max)
        )
        {
            // cout << " : y ";
            nbrFullOverlaps++;
        }

        // cout << endl;
    }

    return nbrFullOverlaps;
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data)
{
    long nbrOverlaps = 0L;

    for(auto val: data)
    {
        // cout << "Pair: " << val ;

        std::stringstream ss(val);

        int p1Min, p1Max;
        int p2Min, p2Max;
        char sep;

        // Extract ranges
        ss >> p1Min >> sep >> p1Max >> sep >> p2Min >> sep >> p2Max;

        auto R2OverlapR1 = [](auto r1Min, auto r1Max, auto r2Min, auto r2Max)
            {
                return
                    r1Min >= r2Min && r1Min <= r2Max  ||
                    r1Max >= r2Min && r1Max <= r2Max;

            };

        // Detect partial overlaps
        if (
            R2OverlapR1 (p1Min, p1Max, p2Min, p2Max)  ||
            R2OverlapR1 (p2Min, p2Max, p1Min, p1Max)
        )
        {
            // cout << " : y ";
            nbrOverlaps++;
        }

        // cout << endl;
    }

    return nbrOverlaps;

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
    std::vector<string> data;

    bool keepSpaces = true;
    bool keepEmptyLines = false;
    myutils::read_file(data, filename, keepSpaces, keepEmptyLines);

    const vector<string> example = {
        "2-4,6-8",
        "2-3,4-5",
        "5-7,7-9",
        "2-8,3-7",
        "6-6,4-6",
        "2-6,4-8"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 2 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 4 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
