//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"

#include <sstream>
#include <map>
#include <set>

using namespace std;

typedef map<string, string> Orbit;

template <typename T>
Orbit constructOrbits(T data)
{
    Orbit orbits;

    for(auto d : data)
    {
        stringstream ss (d);
        string item;
        char delim = ')';

        string object;
        string orbitsAround;

        while (getline (ss, item, delim))
        {
            if(orbitsAround.empty())
                orbitsAround = item;
            else
                object = item;
        }
        orbits.insert ( std::pair<string,string>(object,orbitsAround) );
    }
    return orbits;
}


// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data)
{
    // Construct orbits map
    Orbit orbits = constructOrbits(data);

    // Compute total number of direct and indirect orbits
    int nbrTotOrbits = 0;
    for(auto orbit = orbits.begin(); orbit != orbits.end(); orbit++)
    {
        auto curOrbit = orbit;

        nbrTotOrbits++;

        while(curOrbit->second != "COM")
        {
            nbrTotOrbits++;
            curOrbit = orbits.find(curOrbit->second);
        }
    }

    return nbrTotOrbits;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2(T data)
{
    // Construct orbits map
    Orbit orbits = constructOrbits(data);

    // Find current orbit for YOU
    auto YOU_orbit = orbits.find("YOU");
    auto SAN_orbit = orbits.find("SAN");

#if 1
    // This version is using sets, and set operations: intersection snd difference
    set<string> pathToYOU;
    set<string> pathToSAN;

    // Compute path to YOU
    while(YOU_orbit->second != "COM")
    {
        pathToYOU.insert(YOU_orbit->second);
        YOU_orbit = orbits.find(YOU_orbit->second);
    }

    // Compute path to SAN
    while(SAN_orbit->second != "COM")
    {
        pathToSAN.insert(SAN_orbit->second);
        SAN_orbit = orbits.find(SAN_orbit->second);
    }

    set<string> commonPath;
    set<string> transferYOU;
    set<string> transferSAN;

    // Find common path
    set_intersection(
        pathToYOU.begin(), pathToYOU.end(),
        pathToSAN.begin(), pathToSAN.end(),
        inserter(commonPath, commonPath.begin())
    );

    // Find exclusive path to YOU
    set_difference(
        pathToYOU.begin(), pathToYOU.end(),
        commonPath.begin(), commonPath.end(),
        inserter(transferYOU, transferYOU.begin())
    );

    // Find exclusive path to SAN
    set_difference(
        pathToSAN.begin(), pathToSAN.end(),
        commonPath.begin(), commonPath.end(),
        inserter(transferSAN, transferSAN.begin())
    );

    return transferYOU.size() + transferSAN.size();

#else
    // This version is using iterative backwalks through the orbits map to find a common orbit
    int YOU_transfers = 0;
    int SAN_transfers = 0;

    while(YOU_orbit->second != SAN_orbit->second && YOU_orbit->second != "COM")
    {
        YOU_orbit = orbits.find(YOU_orbit->second);
        YOU_transfers++;

        // Go back from SAN_orbit toward COM. If we find a match with YOU_orbit, jackpot!
        // Otherwise, we change YOU_orbit, and start over.
        SAN_transfers = 0;
        SAN_orbit = orbits.find("SAN");

        while(SAN_orbit->second != "COM")
        {
            SAN_orbit = orbits.find(SAN_orbit->second);
            SAN_transfers++;

            if (YOU_orbit->second == SAN_orbit->second)
            {
                break;
            }
        }
    }
    return YOU_transfers + SAN_transfers;
#endif
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
    auto data = myutils::read_file<string, std::vector<string> >(filename);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const std::vector<string> v1 =
    {
        "COM)B",
        "B)C",
        "C)D",
        "D)E",
        "E)F",
        "B)G",
        "G)H",
        "D)I",
        "E)J",
        "J)K",
        "K)L"
        };
    assert(solve_puzzle1(v1)    ==     42 && "Error verifying puzzle #1");
    assert(solve_puzzle1(data) == 119831 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const std::vector<string> v2 =
    {
        "COM)B",
        "B)C",
        "C)D",
        "D)E",
        "E)F",
        "B)G",
        "G)H",
        "D)I",
        "E)J",
        "J)K",
        "K)L",
        "K)YOU",
        "I)SAN"
    };

    assert(solve_puzzle2(v2)   ==   4 && "Error verifying puzzle #2");
    assert(solve_puzzle2(data) == 322 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
