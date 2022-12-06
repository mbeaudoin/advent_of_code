//
// Copyright (C) Martin Beaudoin. 2022. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <numeric>
#include <algorithm>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data)
{
#if 0
    for(auto var : data)
    {
    	cout<<var<<"\n";
    }
#endif

    std::list<long> calories;
    long sumCalories = 0L;

    for(auto vi : data )
    {
        if(vi.empty())
        {
            calories.push_back(sumCalories);
            sumCalories = 0L;
        }
        else
        {
            long val = std::stol(vi);
            sumCalories += val;
        }
    }
    // Add last block
    calories.push_back(sumCalories);

    // Descending sort
    calories.sort(std::greater<>());

    // Return max value
    return *calories.begin();
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data)
{
#if 0
    for(auto var : data)
    {
    	cout<<var<<"\n";
    }
#endif

    std::vector<long> calories;
    long sumCalories = 0L;

    for(auto vi : data )
    {
        if(vi.empty())
        {
            calories.push_back(sumCalories);
            sumCalories = 0L;
        }
        else
        {
            long val = std::stol(vi);
            sumCalories += val;
        }
    }

    // Add last block
    calories.push_back(sumCalories);

    // Descending sort
    std::sort(calories.begin(),calories.end(), greater<int>());

    // Keep first 3 entries
    calories.resize(3);

    // Return sum
    return std::accumulate(calories.begin(), calories.end(), 0L);

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
    myutils::read_file(data, filename, true, true);

    const vector<string> example = {
        "1000",
        "2000",
        "3000",
        "",
        "4000",
        "",
        "5000",
        "6000",
        "",
        "7000",
        "8000",
        "9000",
        "",
        "10000",
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 24000 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 45000 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;

}
