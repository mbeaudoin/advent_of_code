//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "myutils.h"
#include <sstream>
#include <algorithm>

using namespace std;

typedef struct
{
    int x;
    int y;
} point;

bool operator==(const point &p1, const point& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const point &p1, const point& p2)
{
    return p1.x != p2.x || p1.y != p2.y;
}

std::ostream& operator<<(std::ostream& os, const point& p)
{
    return os << p.x
        << " "
        << p.y;
}

typedef struct
{
    point p1;
    point p2;
} line;

std::ostream& operator<<(std::ostream& os, const line& l)
{
    return os << l.p1
        << " "
        << l.p2;
}

// Compute segment complete list of points
void compute_segments_points(string dirWire, std::vector<point>& wire)
{
    // Make sure we start from a clean list
    wire.clear();

    // Extract list of token from each wire directions
    std::istringstream ss(dirWire);

    std::string token;

    vector<string> lToken;
    while (std::getline(ss, token, ','))
    {
        lToken.push_back(token);
    }

    // Compute wire endpoints
    // Starting point
    point pt = {0, 0};
    wire.push_back(pt);

    for(auto d : lToken)
    {
        char dir = d.front();
        int offset = stoi(d.erase(0, 1));

        for(int i=0; i<offset; i++)
        {
            switch(dir)
            {
                case 'U':
                    pt.y++;
                    break;
                case 'D':
                    pt.y--;
                    break;
                case 'L':
                    pt.x--;
                    break;
                case 'R':
                    pt.x++;
                    break;
            }
            wire.push_back(pt);
        }
    }
}

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data, vector<point>& wire1, vector<point>& wire2, vector<point>& intersections)
{

    point orig({0, 0});

    // Make sure we start from a clean list
    wire1.clear();
    wire2.clear();
    intersections.clear();

    // Extract list of endpoints from each wire directions
    string dirWire1 = data[0];
    string dirWire2 = data[1];

    int minDist = INT_MAX;

    compute_segments_points(dirWire1, wire1);
    compute_segments_points(dirWire2, wire2);

    for(auto w1 : wire1)
    {
        for(auto w2 : wire2)
        {
            if(w1 == w2 && w1 != orig)
                intersections.push_back(w1);
        }
    }

    // Find minimum distance (Manhattan distance)
    for(auto inter : intersections)
        minDist = std::min(minDist, abs(inter.x) + abs(inter.y));

    return minDist;
}

// Solve puzzle #2
int solve_puzzle2(vector<point>& wire1, vector<point>& wire2, vector<point>& intersections)
{
    int minTravelWire1Wire2 = INT_MAX;
    for(auto inter : intersections)
    {
        std::vector<point>::iterator p1 = find(wire1.begin(), wire1.end(), inter);
        std::vector<point>::iterator p2 = find(wire2.begin(), wire2.end(), inter);
        int delta1 = p1 - wire1.begin();
        int delta2 = p2 - wire2.begin();

        minTravelWire1Wire2 = std::min(minTravelWire1Wire2, delta1 + delta2);
    }
    return minTravelWire1Wire2;
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
    auto data = myutils::read_file<string, std::vector<std::string> >(filename);
    //for(auto s : data)
    //    cout << s << endl;

    vector<point> wire1;
    vector<point> wire2;
    vector<point> intersections;
    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>({"R8,U5,L5,D3", "U7,R6,D4,L4"}, wire1, wire2, intersections)                                                          == 6   && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<string>>({"R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83"}, wire1, wire2, intersections)               == 159 && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<string>>({"R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"}, wire1, wire2, intersections) == 135 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, wire1, wire2, intersections) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle1<vector<string>>({"R8,U5,L5,D3", "U7,R6,D4,L4"}, wire1, wire2, intersections) == 6 && "Error verifying puzzle #1");
    assert(solve_puzzle2(wire1, wire2, intersections) == 30 && "Error verifying puzzle #2");

    assert(solve_puzzle1<vector<string>>({"R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83"}, wire1, wire2, intersections) == 159 && "Error verifying puzzle #1");
    assert(solve_puzzle2(wire1, wire2, intersections) == 610 && "Error verifying puzzle #2");

    assert(solve_puzzle1<vector<string>>({"R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"}, wire1, wire2, intersections) == 135 && "Error verifying puzzle #1");
    assert(solve_puzzle2(wire1, wire2, intersections) == 410 && "Error verifying puzzle #2");

    // Solve puzzle #2
    solve_puzzle1(data, wire1, wire2, intersections);
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(wire1, wire2, intersections) << std::endl;
}
