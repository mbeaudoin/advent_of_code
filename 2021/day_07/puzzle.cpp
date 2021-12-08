//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>      // std::stringstream
#include <algorithm>
#include <climits>

#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr long solve_puzzle1(T data)
{
    vector<int> crabPos(0);

    std::stringstream ss(data[0]);
    std::string token;

    while(std::getline(ss, token, ','))
    {
        crabPos.push_back(stoi(token));
    }

    const auto [posMin, posMax] = std::minmax_element(begin(crabPos), end(crabPos));

    long minEnergy = INT_MAX;

    for(int i=*posMin; i<= *posMax; i++)
    {
        long energy = 0;
        for(auto c : crabPos)
        {
            energy += std::abs(c - i);
        }

        minEnergy = std::min(energy, minEnergy);
    }

    return minEnergy;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    vector<int> crabPos(0);

    std::stringstream ss(data[0]);
    std::string token;

    while(std::getline(ss, token, ','))
    {
        crabPos.push_back(stoi(token));
    }

    const auto [posMin, posMax] = std::minmax_element(begin(crabPos), end(crabPos));

    long minEnergy = INT_MAX;

    // lambda function: sum of n consecutive integers
    auto sumN = [](int n) ->long {
        long sum = 0;
        for(int i = 1; i<=n; i++)
        {
            sum += i;
        }
        return sum;
    };

    for(int i=*posMin; i<= *posMax; i++)
    {
        long energy = 0;
        for(auto c : crabPos)
        {
            energy += sumN(std::abs(c - i));
        }

        minEnergy = std::min(energy, minEnergy);
    }

    return minEnergy;
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
    const std::vector<std::string> example1 = {"16,1,2,0,4,2,7,1,2,14"};
    assert(solve_puzzle1<vector<string>>(example1) == 37 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const auto example2 = "2";
    assert(solve_puzzle2<vector<string>>(example1) == 168 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
