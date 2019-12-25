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
#include <iomanip>

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
    int  x,  y,  z;  // Position
    int vx, vy, vz;  // Velocity

    moonMotion()
        : x(0), y(0), z(0), vx(0), vy(0), vz(0)
        {}

    moonMotion(int mx, int my, int mz, int mvx = 0, int mvy = 0, int mvz = 0)
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
    int vsize = m1.size();

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

    vector<int> numVals;
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
    int delta;

    for(int index1 = 0; index1 < moons.size(); index1++)
    {
        moonMotion& m1 = moons[index1];
        for(int index2 = index1+1; index2 < moons.size(); index2++)
        {
            moonMotion& m2 = moons[index2];

            delta = myutils::sgn(m1.x - m2.x);
            m1.vx -= delta;
            m2.vx += delta;

            delta = myutils::sgn(m1.y - m2.y);
            m1.vy -= delta;
            m2.vy += delta;

            delta = myutils::sgn(m1.z - m2.z);
            m1.vz -= delta;
            m2.vz += delta;
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
    }
    return totEnergy;
}

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data, int nbrTimeSteps)
{
    vector<moonMotion> moons = extractMoonsPos(data);
    long totalEnergy = 0;

    for(int t=1; t<=nbrTimeSteps; t++)
    {
        computeGravity (moons);
        computeVelocity(moons);
        totalEnergy = computeEnergy(moons);
    }

    return totalEnergy;
}

// Floyd algorithm for finding cycle.
void floyd_cycle_find(
    vector<moonMotion>& moons,
    long long& lambda,              // Loop length
    long long& mu                   // Index of first encounter
)
{
    long long nbrTimeSteps = 0;

    // Starting point
    vector<moonMotion> moonsTurtle = moons;
    vector<moonMotion> moonsHare = moons;

    // First part of Floyd algorithm
    do
    {
        // Evaluate turtle
        computeGravity (moonsTurtle);
        computeVelocity(moonsTurtle);

        // Evaluate hare (goes twice as fast)
        computeGravity (moonsHare);
        computeVelocity(moonsHare);
        computeGravity (moonsHare);
        computeVelocity(moonsHare);

        nbrTimeSteps++;
    }
    while(moonsTurtle != moonsHare);

    moonsTurtle = moons;

    mu = 0;
    while(moonsTurtle != moonsHare)
    {
        // Evaluate turtle
        computeGravity (moonsTurtle);
        computeVelocity(moonsTurtle);

        // Evaluate hare (goes same speed as turtle)
        computeGravity (moonsHare);
        computeVelocity(moonsHare);

        mu++;
        nbrTimeSteps++;
    }

    lambda = nbrTimeSteps - mu;

    return;
}

// Brent algorithm for finding cycle.
// For this specific problem, this variant of the algorithm is not faster than Floyd algorithm
void brent_cycle_find(
    vector<moonMotion>& moons,
    long long& lambda,              // Loop length
    long long& mu                   // Index of first encounter
)
{
    // Starting point
    long long power;
    power  = 1;
    lambda = 1;

    vector<moonMotion> moonsTurtle = moons;
    vector<moonMotion> moonsHare = moons;

    // Evaluate hare
    computeGravity (moonsHare);
    computeVelocity(moonsHare);

    while(moonsTurtle != moonsHare)
    {
        if(power == lambda)
        {
            moonsTurtle = moonsHare;
            power *= 2;
            lambda = 0;
        }

        computeGravity (moonsHare);
        computeVelocity(moonsHare);
        lambda++;
    }

    moonsTurtle = moons;
    moonsHare = moons;

    mu = 0;

    while(moonsTurtle != moonsHare)
    {
        computeGravity (moonsTurtle);
        computeVelocity(moonsTurtle);

        computeGravity (moonsHare);
        computeVelocity(moonsHare);
        mu++;
    }
    return;
}

// Solve puzzle #2
//
// This is solved using the Floyd algorithm for finding cycles in a sequence.
// https://en.wikipedia.org/wiki/Cycle_detection#Floyd's_Tortoise_and_Hare
//
template <typename T>
long long solve_puzzle2(T data)
{
    long long lambdaX, lambdaY, lambdaZ;   // Loop length
    long long muX, muY, muZ;               // Index of first encounter

    // First, we compute the initial state of each moon
    vector<moonMotion> moons = extractMoonsPos(data);

    // x, y and z coords are moving independantly, so compute the frequencies of each coordinates
    vector<moonMotion> moonsX = moons;
    vector<moonMotion> moonsY = moons;
    vector<moonMotion> moonsZ = moons;

    for(auto &m : moonsX)
    {
        m.y = 0;
        m.z = 0;
        m.vy = 0;
        m.vz = 0;
    }

    for(auto &m : moonsY)
    {
        m.x = 0;
        m.z = 0;
        m.vx = 0;
        m.vz = 0;
    }

    for(auto &m : moonsZ)
    {
        m.x = 0;
        m.y = 0;
        m.vx = 0;
        m.vy = 0;
    }

    cout << "Solving puzzle #2 using Floyd algorithm" << endl;
    floyd_cycle_find(moonsX, lambdaX, muX);
    floyd_cycle_find(moonsY, lambdaY, muY);
    floyd_cycle_find(moonsZ, lambdaZ, muZ);

    // The full period is the lcm of the 3 lambdas
    return lcm(lambdaX, lcm(lambdaY, lambdaZ));
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
    assert(solve_puzzle2(data) == 583523031727256 && "Error verifying puzzle #2");
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
