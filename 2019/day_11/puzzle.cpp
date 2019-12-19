//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"
#include <map>
#include <set>

#include "Intcode.h"

using namespace std;

typedef myutils::point coords;

typedef enum
{
    NORTH = 0,
    SOUTH = 1,
    EAST  = 2,
    WEST  = 3
} direction;

std::ostream& operator<<(std::ostream& os, const direction& dir)
{
    string dirStr;
    switch (dir)
    {
        case NORTH:
            dirStr = "North";
            break;
        case SOUTH:
            dirStr = "South";
            break;
        case EAST:
            dirStr = "East";
            break;
        case WEST:
            dirStr = "West";
            break;
    }

    return os << dirStr;
}

long encodePosition(coords p)
{
    return 1000*p.y + p.x;
}

// Solve puzzle #1
template <typename T>
long long solve_puzzle1(T data, int startingColor, bool plotTrace, bool debug)
{
    if(debug)
    {
        cout << endl << "Solve puzzle1:" << endl;

        for(auto d : data)
            cout << d << " : " ;
        cout << endl;
    }

    // Initialize painting program
    Intcode computer(data, startingColor);

    // Control the flow of input/output
    computer.setPipeOutputMode(true);

    // Initialize painting area. Encode position
    map<long, int> hull;
    map<long, string, std::greater<long> > hullStr;

    // Robot position and direction
    coords curPos(0, 0);             // Start at (0, 0)
    direction curDir(NORTH);   // Point up toward +y

    // Precompute all combination of direction changes
    map<direction, coords> dirNum;

    dirNum[NORTH] = coords( 0,  1);
    dirNum[SOUTH] = coords( 0, -1);
    dirNum[EAST]  = coords( 1,  0);
    dirNum[WEST]  = coords(-1,  0);

    map<direction, vector<direction> > dirChanges;
    // index 0: left  90 degrees
    // index 1: right 90 degrees
    dirChanges[NORTH].push_back(WEST);  // From ^ to <
    dirChanges[NORTH].push_back(EAST);  // From ^ to >
    dirChanges[SOUTH].push_back(EAST);  // From v to >
    dirChanges[SOUTH].push_back(WEST);  // From v to <

    dirChanges[EAST].push_back(NORTH);  // From > to ^
    dirChanges[EAST].push_back(SOUTH);  // From > to v
    dirChanges[WEST].push_back(SOUTH);  // From < to v
    dirChanges[WEST].push_back(NORTH);  // From < to ^

    // Set curent color
    long long currentColor = startingColor;

    // Run until halted
    while(!computer.isHalted())
    {
        long long colorToPaint = computer.run();
        long long nextDir      = computer.run();

        if(debug)
            cout << "curPos: " << curPos << " : current color: " << currentColor << " : colorToPaint: " << colorToPaint << " : nextDir: " << dirChanges[curDir][nextDir] << endl;

        // Numeric hull
        hull[encodePosition(curPos)] = colorToPaint;

        if(plotTrace)
        {
            // String hull
            string curStr = hullStr.find(curPos.y) == hullStr.end() ? "" : hullStr[curPos.y];

            if(curStr.size() < curPos.x + 1)
                curStr.resize(curPos.x + 1, ' ');
            curStr[curPos.x] = (colorToPaint == 1 ? '*' : ' ');
            hullStr[curPos.y] = curStr;
        }

        // Change direction and position
        curDir = dirChanges[curDir][nextDir];
        curPos = curPos + dirNum[curDir];

        // We just move. Adjust current color
        currentColor = hull.find(encodePosition(curPos)) == hull.end() ? 0 : hull[encodePosition(curPos)];

        // Tell computer current color
        computer.setInput({currentColor});
    }

    if(plotTrace)
    {
        cout << endl << "We plot the robot trace:"<< endl;

        for(auto h : hullStr)
            cout << h.second << endl;
    }

    return hull.size();
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, bool plotTrace, bool debug = false)
{
    return solve_puzzle1(data, 1, plotTrace, debug);
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
    std::vector<long long> data = myutils::read_file_csv<long long, std::vector<long long> >(filename);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(data, 0, false, false) == 1964 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 0, false, false) << std::endl;

    // --------- Puzzle #2 ---------
    assert(solve_puzzle2(data, false, false) == 250 && "Error verifying puzzle #2");

    // Solve puzzle #2 : FKEKCFRK
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data, true) << std::endl;
}
