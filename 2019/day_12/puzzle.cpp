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
#include <algorithm>
#include <numeric>

#include <boost/functional/hash.hpp>

using namespace std;

typedef enum
{
    IO       = 0,
    EUROPA   = 1,
    GANYMEDE = 2,
    CALLISTO = 3
} Moons;

typedef struct moonMotion
{
    short  x,  y,  z;  // Position
    short vx, vy, vz;  // Velocity

    moonMotion()
        : x(0), y(0), z(0), vx(0), vy(0), vz(0)
        {}

    moonMotion(short mx, short my, short mz, short mvx = 0, short mvy = 0, short mvz = 0)
        : x(mx), y(my), z(mz), vx(mvx), vy(mvy), vz(mvz)
        {}

    moonMotion(const moonMotion& m)
        : x(m.x), y(m.y), z(m.z), vx(m.vx), vy(m.vy), vz(m.vz)
        {}

    bool operator!=(const moonMotion &other)
    {
        return
            x  != other.x  ||
            y  != other.y  ||
            z  != other.z  ||
            vx != other.vx ||
            vy != other.vy ||
            vz != other.vz;
    }

    bool operator==(const moonMotion &other)
    {
        return
            x  == other.x  &&
            y  == other.y  &&
            z  == other.z  &&
            vx == other.vx &&
            vy == other.vy &&
            vz == other.vz;
    }

} moonMotion;

std::ostream& operator<<(std::ostream& os, const moonMotion& mm)
{
    return os << std::setw(10)
        << "pos=<x=" << std::setw(3) << mm.x << ", "
        << "y="      << std::setw(3) << mm.y << ", "
        << "z="      << std::setw(3) << mm.z << ">, "
        << "vel=<x=" << std::setw(3) << mm.vx << ", "
        << "y="      << std::setw(3) << mm.vy << ", "
        << "z="      << std::setw(3) << mm.vz << ">";
}

bool operator !=(const vector<moonMotion>& m1, const vector<moonMotion>& m2)
{
    bool isSame = true;
    short vsize = m1.size();

    for(int i=0; isSame && i<vsize; i++)
        isSame &= (m1[i].x  == m2[i].x);

    for(int i=0; isSame && i<vsize; i++)
        isSame &= (m1[i].y  == m2[i].y);

    for(int i=0; isSame && i<vsize; i++)
        isSame &= (m1[i].z  == m2[i].z);

    for(int i=0; isSame && i<vsize; i++)
        isSame &= (m1[i].vx  == m2[i].vx);

    for(int i=0; isSame && i<vsize; i++)
        isSame &= (m1[i].vy  == m2[i].vy);

    for(int i=0; isSame && i<vsize; i++)
        isSame &= (m1[i].vz  == m2[i].vz);

    return !isSame;
}


template <typename T>
vector<moonMotion>
extractMoonsPos(T data)
{
    //for(auto d : data)
    //    cout << d << endl;

    vector<moonMotion> moons;

    vector<short> numVals;
    int moonIndex = 0;
    for(auto d : data)
    {
        // Remove junk
        d.erase(std::remove(d.begin(), d.end(), ','), d.end());
        d.erase(std::remove(d.begin(), d.end(), '>'), d.end());

        string token;
        std::stringstream tokenSplitter(d);
        getline(tokenSplitter, token, '=');
        getline(tokenSplitter, token, '=');

        numVals.push_back(stoi(token));
    }

    moonMotion Io      (numVals[0], numVals[1],  numVals[2]);
    moonMotion Europa  (numVals[3], numVals[4],  numVals[5]);
    moonMotion Ganymede(numVals[6], numVals[7],  numVals[8]);
    moonMotion Callisto(numVals[9], numVals[10], numVals[11]);

    moons.push_back(Io);
    moons.push_back(Europa);
    moons.push_back(Ganymede);
    moons.push_back(Callisto);

    return moons;
}

void
computeGravity(vector<moonMotion>& moons)
{
    // Compute delta gravity from current position
    vector<moonMotion> deltaGravity(moons.size());

    for(int index1 = 0; index1 < moons.size(); index1++)
    {
        moonMotion& m1 = moons[index1];
        for(int index2 = index1+1; index2 < moons.size(); index2++)
        {
            moonMotion& m2 = moons[index2];
            if (m1.x != m2.x)
            {
                if (m1.x < m2.x)
                {
                    m1.vx++;
                    m2.vx--;
                }
                else
                {
                    m1.vx--;
                    m2.vx++;
                }
            }

            if (m1.y != m2.y)
            {
                if (m1.y < m2.y)
                {
                    m1.vy++;
                    m2.vy--;
                }
                else
                {
                    m1.vy--;
                    m2.vy++;
                }
            }

            if (m1.z != m2.z)
            {
                if (m1.z < m2.z)
                {
                    m1.vz++;
                    m2.vz--;
                }
                else
                {
                    m1.vz--;
                    m2.vz++;
                }
            }
        }
    }

    return;
}

void
computeVelocity(vector<moonMotion>& moons)
{
    for(auto& m : moons)
    {
        m.x += m.vx;
        m.y += m.vy;
        m.z += m.vz;
    }
    return;
}

long
computeEnergy(vector<moonMotion>& moons)
{
    long totEnergy = 0;
    long potEnergy = 0;
    long kinEnergy = 0;
    for(auto& m : moons)
    {
        potEnergy = abs(m.x)  + abs(m.y)  + abs(m.z);
        kinEnergy = abs(m.vx) + abs(m.vy) + abs(m.vz);

        totEnergy += potEnergy * kinEnergy;

        cout << "pot: " << std::setw(3) << potEnergy
            << " : kin: " << std::setw(3) << kinEnergy
            << " tot: " << std::setw(3) << potEnergy * kinEnergy << endl;
    }

    cout << "---------> Sum of total energy: " << totEnergy << endl;

    return totEnergy;
}

size_t computeHashValue(vector<moonMotion>& moons)
{
    std::size_t seed = 0;

    for(auto m : moons)
    {
        boost::hash_combine(seed, m.x);
        boost::hash_combine(seed, m.y);
        boost::hash_combine(seed, m.z);
        boost::hash_combine(seed, m.vx);
        boost::hash_combine(seed, m.vy);
        boost::hash_combine(seed, m.vz);
    }
    return seed;
}

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data, int nbrTimeSteps)
{
    vector<moonMotion> moons = extractMoonsPos(data);
    long totalEnergy = 0;

    // Do the motions
    cout << "After 0  steps:" << endl;
    for(auto m : moons)
        cout << m << endl;

    for(int t=1; t<=nbrTimeSteps; t++)
    {
        computeGravity (moons);
        computeVelocity(moons);

        cout << "After " << t << " steps:" << endl;
        for(auto m : moons)
            cout << m << endl;

        totalEnergy = computeEnergy(moons);
    }

    return totalEnergy;
}

// Solve puzzle #2
//
// This is solved using the Floyd algorithm for finding cycles in a sequence.
// https://en.wikipedia.org/wiki/Cycle_detection#Floyd's_Tortoise_and_Hare
//
template <typename T>
long long solve_puzzle2(T data)
{
    cout << "Solving puzzle #2" << endl;

    long long nbrTimeSteps = 0;

    // First, we compute the initial state of each moon
    vector<moonMotion> moons = extractMoonsPos(data);

    vector<moonMotion> moonsTortoise = moons;
    vector<moonMotion> moonsHare = moons;

    size_t tortoise, hare;     // The 2 pointers from Floyd's algorithm

    // Starting point
    //tortoise = computeHashValue(moonsTortoise);
    hare = tortoise;

    // First part of Floyd algorithm
    do
    {
        // Evaluate tortoise
        computeGravity (moonsTortoise);
        computeVelocity(moonsTortoise);
        //tortoise = computeHashValue(moonsTortoise);

        // Evaluate hare (goes twice as fast)
        computeGravity (moonsHare);
        computeVelocity(moonsHare);
        computeGravity (moonsHare);
        computeVelocity(moonsHare);
        //hare = computeHashValue(moonsHare);

        nbrTimeSteps++;

        if(nbrTimeSteps % 1000000 == 0)
            cout << setw(11) << nbrTimeSteps << endl << std::flush;
    }
    //while(tortoise != hare);
    while(moonsTortoise != moonsHare);

    cout << "First part: found matching hare and tortoise at timestep: " << nbrTimeSteps << endl;

    // Second part of Floyd algorithm
    moonsTortoise = moons;
    //tortoise = computeHashValue(moonsTortoise);

    while(moonsTortoise != moonsHare)
    //while(tortoise != hare)
    {
        // Evaluate tortoise
        computeGravity (moonsTortoise);
        computeVelocity(moonsTortoise);
        //tortoise = computeHashValue(moonsTortoise);

        // Evaluate hare (goes same speed as tortoise)
        computeGravity (moonsHare);
        computeVelocity(moonsHare);
        //hare = computeHashValue(moonsHare);

        nbrTimeSteps++;

        if(nbrTimeSteps % 1000000 == 0)
            cout << setw(11) << nbrTimeSteps << endl << std::flush;
    }

    cout << "Second part: found matching hare and tortoise at timestep: " << nbrTimeSteps << endl;


    return nbrTimeSteps;
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

    // Example1
    vector<string> dataEx1 = {
        "<x=-1",
        "y=0",
        "z=2>",
        "<x=2",
        "y=-10",
        "z=-7>",
        "<x=4",
         "y=-8",
         "z=8>",
        "<x=3",
         "y=5",
         "z=-1>"
    };
    long nTimeSteps = 10;
    //assert(solve_puzzle1(dataEx1, nTimeSteps) == 179 && "Error verifying puzzle #1");

    // Example2
    vector<string> dataEx2 = {
        "<x=-8",
        "y=-10",
        "z=0>",
        "<x=5",
        "y=5",
        "z=10>",
        "<x=2",
         "y=-7",
         "z=3>",
        "<x=9",
         "y=-8",
         "z=-3>"
    };

    nTimeSteps = 100;
    assert(solve_puzzle1(dataEx2, nTimeSteps) == 1940 && "Error verifying puzzle #1");

    // Solve puzzle #1
    nTimeSteps = 1000;
    assert(solve_puzzle1(data, nTimeSteps) == 10635 && "Error verifying puzzle #1");

    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, nTimeSteps) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(dataEx1) ==       2772 && "Error verifying puzzle #2");
    assert(solve_puzzle2(dataEx2) == 4686774924 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
