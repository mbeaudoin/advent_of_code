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
int solve_puzzle1(int nMin, int nMax)
{
    int counter = 0;
    while(nMin <= nMax)
    {
        int d1 = nMin/100000 % 10;
        int d2 = nMin/10000 % 10;
        int d3 = nMin/1000 % 10;
        int d4 = nMin/100 % 10;
        int d5 = nMin/10 % 10;
        int d6 = nMin % 10;

        //cout << d1 << " : " << d2  << " : " << d3  << " : " << d4  << " : " << d5  << " : " << d6 << endl;

        if (
            (d1 == d2 || d2 == d3 || d3 == d4 || d4 == d5 || d5 == d6) &&
            ( d1 <= d2 && d2 <= d3 && d3 <= d4 && d4 <= d5 && d5 <= d6)
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
    //cout << "nMin: " << nMin << endl;
    //cout << "nMax: " << nMax << endl;

    int counter = 0;
    do
    {
        int d1 = nMin/100000 % 10;
        int d2 = nMin/10000 % 10;
        int d3 = nMin/1000 % 10;
        int d4 = nMin/100 % 10;
        int d5 = nMin/10 % 10;
        int d6 = nMin % 10;

        // cout << d1 << " : " << d2  << " : " << d3  << " : " << d4  << " : " << d5  << " : " << d6 << endl;

        if (
            (d1 == d2 || d2 == d3 || d3 == d4 || d4 == d5 || d5 == d6) &&
            ( d1 <= d2 && d2 <= d3 && d3 <= d4 && d4 <= d5 && d5 <= d6)
        )
        {
            // This is ugly as hell, and will never scale....
            // Need to find a better way than this exhaustive craziness
            if (
                (!(d1 == d2 && d2 == d3)                        || (d4 == d5 && d4 != d1) || (d5 == d6 && d5 != d1)) &&
                (!(d2 == d3 && d3 == d4)                        || (d5 == d6 && d5 != d2 ))                          &&
                (!(d3 == d4 && d4 == d5)                        || (d1 == d2 && d1 != d3 ))                          &&
                (!(d4 == d5 && d5 == d6)                        || (d1 == d2 && d1 != d4) || (d2 == d3 && d2 != d4)) &&
                (!(d1 == d2 && d2 == d3 && d3 == d4)            || (d5 == d6 && d5 != d1))                           &&
                (!(d2 == d3 && d3 == d4 && d4 == d5)                         )                                       &&
                (!(d3 == d4 && d4 == d5 && d5 == d6)            || (d1 == d2 && d2 != d3))                           &&
                (!(d1 == d2 && d2 == d3 && d3 == d4 && d4 == d5))                                                    &&
                (!(d2 == d3 && d3 == d4 && d4 == d5 && d5 == d6))                                                    &&
                (!(d1 == d2 && d2 == d3 && d4 == d5 && d5 == d6))
            )
            {
                //cout << nMin << endl;
                counter++;
            }
        }

        nMin++;
    }
    while (nMin <= nMax);

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
    const auto example1 = 1;
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
