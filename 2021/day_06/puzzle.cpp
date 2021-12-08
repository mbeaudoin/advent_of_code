//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include <climits>
#include "myutils.h"

using namespace std;

long growthSingleFish(int t0, int timeMax)
{
    long popFromThisFish = 0;

    for(int t = t0+1; t<=timeMax; t += 7)
    {
        //cout << "Time for a new fish : "  << t << endl;
        popFromThisFish += 1 + growthSingleFish(t+ 2 + 6, timeMax);
    }

    return popFromThisFish;
}

// Solve puzzle #1
template <typename T>
constexpr long solve_puzzle1(T data, int timeMax)
{
    vector<unsigned char> fishes(0);

    std::stringstream ss(data[0]);
    std::string token;

    while(std::getline(ss, token, ','))
    {
        fishes.push_back(stoi(token));
    }

    // Initial population
    long populationSize = fishes.size();;

    // Growth from initial population
    int counter = 0;
    map<int, long> cacheComputation;
    for(auto f : fishes)
    {
        long nbrKids;

        if(cacheComputation.find(f) == cacheComputation.end())
        {
            nbrKids = growthSingleFish(f, timeMax);
            cacheComputation[f] = nbrKids;
        }

        populationSize += cacheComputation[f];

        cout << "fish#: " << counter++
            << " : f: "  << (int)f
            << " : nbrKids: " << cacheComputation[f]
            << " : tot: "  << populationSize
            << endl;
    }

    cout << "Population after "  << timeMax << " days: " << populationSize << endl;

    return populationSize;
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
    vector<string> data;
    myutils::read_file(data, filename, true, false);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const std::vector<std::string> example1 = {"3,4,3,1,2"};
    assert(solve_puzzle1(example1, 18) == 26 && "Error verifying puzzle #1");
    assert(solve_puzzle1(example1, 80) == 5934 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 80) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle1(example1, 256) == 26984457539 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle1(data, 256) << std::endl;
}
