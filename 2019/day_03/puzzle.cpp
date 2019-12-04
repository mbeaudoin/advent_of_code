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

typedef vector<line> polyline;

point find_intersection(
    line l1,
    line l2)
{
    point intersection = {0, 0};

    int l1Xmin = std::min(l1.p1.x, l1.p2.x);
    int l1Xmax = std::max(l1.p1.x, l1.p2.x);
    int l2Xmin = std::min(l2.p1.x, l2.p2.x);
    int l2Xmax = std::max(l2.p1.x, l2.p2.x);

    int l1Ymin = std::min(l1.p1.y, l1.p2.y);
    int l1Ymax = std::max(l1.p1.y, l1.p2.y);
    int l2Ymin = std::min(l2.p1.y, l2.p2.y);
    int l2Ymax = std::max(l2.p1.y, l2.p2.y);

    if(
        l1Xmin >= l2Xmin && l1Xmin <= l2Xmax &&
        l1Xmax >= l2Xmin && l1Xmax <= l2Xmax &&
        l2Ymin >= l1Ymin && l2Ymin <= l1Ymax &&
        l2Ymax >= l1Ymin && l2Ymax <= l1Ymax
    )
    {
        intersection = {l1Xmin, l2Ymin};
    }
    else if(
        l2Xmin >= l1Xmin && l2Xmin <= l1Xmax &&
        l2Xmax >= l1Xmin && l2Xmax <= l1Xmax &&
        l1Ymin >= l2Ymin && l1Ymin <= l2Ymax &&
        l1Ymax >= l2Ymin && l1Ymax <= l2Ymax
    )
    {
        intersection = {l2Xmin, l1Ymin};
    }
    return intersection;
}

// Compute segment endpoints
void compute_segments_endpoints(string dirWire, polyline& wire)
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
    point p1 = {0, 0};
    point p2 = {0, 0};

    for(auto d : lToken)
    {
        char dir = d.front();
        int offset = stoi(d.erase(0, 1));


        switch(dir)
        {
            case 'U':
                p2.y += offset;
                break;
            case 'D':
                p2.y -= offset;
                break;
            case 'L':
                p2.x -= offset;
                break;
            case 'R':
                p2.x += offset;
                break;
        }

        wire.push_back({p1, p2});

        p1 = p2;
    }
}

vector<int>
find_travel_distance(vector<point>& intersections, polyline& wire)
{
    vector<int> travelToIntersection;

    for(auto inter : intersections)
    {

        int travelWire = 0;
        for(auto w : wire)
        {
            int curx = w.p1.x;
            int cury = w.p1.y;
            int incrx = 0;
            int incry = 0;
            int travelSegment = 0;
            while(curx != w.p2.x || cury != w.p2.y)
            {
                if(w.p1.x < w.p2.x)
                    curx++;
                else if (w.p1.x > w.p2.x)
                    curx--;

                if(w.p1.y < w.p2.y)
                    cury++;
                else if (w.p1.y > w.p2.y)
                    cury--;

                travelSegment++;

                if(curx == inter.x && cury == inter.y)
                {
                    // Found an intersection
                    travelToIntersection.push_back(travelWire + travelSegment);
                    break;
                }
            }

            travelWire += travelSegment;
        }
    }
    return travelToIntersection;
}


// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data, polyline& wire1, polyline& wire2, vector<point>& intersections)
{
    // Make sure we start from a clean list
    wire1.clear();
    wire2.clear();
    intersections.clear();

    // Extract list of token from each wire directions
    string dirWire1 = data[0];
    string dirWire2 = data[1];

    compute_segments_endpoints(dirWire1, wire1);
    compute_segments_endpoints(dirWire2, wire2);

    // Find intersections
    for(auto l1 : wire1)
    {
        for(auto l2: wire2)
        {
            point intersection = find_intersection(l1, l2);
            if(intersection.x != 0 || intersection.y != 0)
                intersections.push_back(intersection);
        }
    }

    // Find minimum distance (Manhattan distance)
    int minDist = INT_MAX;

    for(auto inter : intersections)
    {
        minDist = std::min(minDist, abs(inter.x) + abs(inter.y));
        // cout << "Min dist: " << minDist << endl;
    }

    return minDist;
}

// Solve puzzle #2
int solve_puzzle2(polyline& wire1, polyline& wire2, vector<point>& intersections)
{
    // We march each wire until we reach a given intersection, then we compute the minimal marching length
    vector<int> travelToIntersectionWire1 = find_travel_distance(intersections, wire1);
    vector<int> travelToIntersectionWire2 = find_travel_distance(intersections, wire2);

    int count = 0;
    int minTravelWire1Wire2 = INT_MAX;
    for(auto inter : intersections)
    {
        minTravelWire1Wire2 = std::min(minTravelWire1Wire2, travelToIntersectionWire1[count] + travelToIntersectionWire2[count]);
        count++;
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

    polyline wire1;
    polyline wire2;
    vector<point> intersections;
    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>({"R8,U5,L5,D3", "U7,R6,D4,L4"}, wire1, wire2, intersections) == 6 && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<string>>({"R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83"}, wire1, wire2, intersections) == 159 && "Error verifying puzzle #1");
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
