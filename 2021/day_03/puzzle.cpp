//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <list>
#include <bitset>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr long solve_puzzle1(T diag)
{
    const int nBits = diag[0].size();
    const int halfDiagSize = diag.size()/2;

    vector<int> bitSums(nBits, 0);

    std::bitset<16> gammaRate;
    std::bitset<16> epsilonRate;

    for(auto entry : diag)
    {
        std::stringstream strstr(entry);

        std::bitset<16> bit_entry;

        strstr >> bit_entry;

        //cout << bit_entry << endl;

        for(int i=0; i<nBits; i++)
        {
            bitSums[i] += bit_entry[i];
        }
    }

    for(int i=0; i<nBits; i++)
    {
        if(bitSums[i]  > halfDiagSize)
        {
            gammaRate[i] = 1;
        }
        else
        {
            epsilonRate[i] = 1;
        }
    }

    // cout << "gammaRate  : " << gammaRate << " : " << gammaRate.to_ulong() << endl;
    // cout << "epsilonRate: " << epsilonRate << " : " << epsilonRate.to_ulong() << endl;

    return gammaRate.to_ulong() * epsilonRate.to_ulong();
}

// Solve puzzle #2
template <typename T>
constexpr long solve_puzzle2(T diag)
{
    const int nBits = diag[0].size();

    list<bitset<16> > diagBit;

    // Convert array to bitsets
    for(int i=0; i<diag.size(); i++)
    {
        bitset<16> d;
        std::stringstream strstr(diag[i]);
        strstr >> d;
        diagBit.push_back(d);
    }

    // Let's sort...
    list<bitset<16> > sortedO2(diagBit);
    list<bitset<16> > sortedCO2(diagBit);

    for(int i=nBits-1; i>=0; i--)
    {
        // Sorting O2
        if(sortedO2.size() > 1)
        {
            list<bitset<16> > hasOne;
            list<bitset<16> > hasZero;

            for(auto o2 : sortedO2)
            {
                o2[i] == 1 ? hasOne.push_back(o2) :  hasZero.push_back(o2);
            }

            sortedO2 = (hasOne.size() >= hasZero.size()) ? hasOne : hasZero;
        }
        // Sorting CO2
        if(sortedCO2.size() > 1)
        {
            list<bitset<16> > hasOne;
            list<bitset<16> > hasZero;

            for(auto co2 : sortedCO2)
            {
                co2[i] == 1 ? hasOne.push_back(co2) :  hasZero.push_back(co2);
            }

            sortedCO2 = (hasZero.size() <= hasOne.size()) ? hasZero : hasOne;
        }
    }

    int oxygenGeneratorRating = sortedO2.front().to_ulong();
    int co2ScrubberRating = sortedCO2.front().to_ulong();

    //cout << "oxygenGeneratorRating: "  << oxygenGeneratorRating << endl;
    //cout << "co2ScrubberRating: "  << co2ScrubberRating << endl;

    return oxygenGeneratorRating * co2ScrubberRating;
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
    const std::vector<std::string> example1 = {
        "00100",
        "11110",
        "10110",
        "10111",
        "10101",
        "01111",
        "00111",
        "11100",
        "10000",
        "11001",
        "00010",
        "01010",
    };

    assert(solve_puzzle1<vector<string>>(example1) == 198 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const auto example2 = "2";
    assert(solve_puzzle2<vector<string>>(example1) == 230 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
