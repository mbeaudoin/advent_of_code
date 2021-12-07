//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <vector>
#include <sstream>      // std::stringstream
#include <bits/stdc++.h>

#include "myutils.h"

using namespace std;

class Coords
{
public:
    int x;
    int y;
};

class Line
{
public:
    Coords p1;
    Coords p2;

    // Constructor
    Line () {};

    Line(int x1, int y1, int x2, int y2)
    {
        p1.x = x1;
        p1.y = y1;
        p2.x = x2;
        p2.y = y2;
    }

    // Copy constructor
    Line(const Line &l)
    {
        p1.x = l.p1.x;
        p1.y = l.p1.y;
        p2.x = l.p2.x;
        p2.y = l.p2.y;
    }

    bool isHorizontal()
    {
        return (p1.y == p2.y);
    }

    bool isVertical()
    {
        return (p1.x == p2.x);
    }

    bool isDiagonal()
    {
        return std::abs(p1.x - p2.x) == std::abs(p1.y - p2.y);
    }

    friend ostream& operator<<(ostream& os, const Line& line);

};

ostream& operator<<(ostream& os, const Line& line)
{
    os << line.p1.x
        << "," << line.p1.y
        << " -> "
        << line.p2.x << "," << line.p2.y
        << endl;
    return os;
}


// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    // Extract line info

    list<Line> lines(0);

    int xmax = INT_MIN;
    int ymax = INT_MIN;

    for(auto s : data)
    {
        // cout << s << endl;
        int x1, y1, x2, y2;
        string s_scrap;
        char   c_scrap;

        std::stringstream ss(s);
        ss >> x1 >> c_scrap >> y1 >> s_scrap >> x2 >> c_scrap >> y2;
        //cout << " x1: " <<  x1 << endl;
        //cout << " y1: " <<  y1 << endl;
        //cout << " x2: " <<  x2 << endl;
        //cout << " y2: " <<  y2 << endl;

        xmax = std::max(xmax, std::max(x1, x2));
        ymax = std::max(ymax, std::max(y1, y2));

        Line l(x1, y1, x2, y2);

        lines.push_back(l);
    }

    // Force min coords at 0,0... Might not be optimal
    int map_deltax = xmax + 1;
    int map_deltay = ymax + 1;

    vector<int> map(map_deltax * map_deltay, 0);

    for(auto l : lines)
    {
        if(l.isHorizontal())
        {
            int xmin = std::min(l.p1.x, l.p2.x);
            int xmax = std::max(l.p1.x, l.p2.x);
            int y = l.p1.y;

            for(int x = xmin; x <= xmax; x++)
            {
                map[y*map_deltax + x] += 1;
            }

        }
        else if(l.isVertical())
        {
            int ymin = std::min(l.p1.y, l.p2.y);
            int ymax = std::max(l.p1.y, l.p2.y);
            int x = l.p1.x;

            for(int y = ymin; y <= ymax; y++)
            {
                map[y*map_deltax + x] += 1;
            }
        }
    }

    int nbrOverlap = 0;

    for(auto m : map)
    {
        if(m >= 2)
            nbrOverlap++;
    }

#if 0
    // Show the map
    int counter = 1;
    for(auto m : map)
    {
        cout << m << " ";

        if(counter++ % (deltax) == 0)
        {
            cout << endl;
        }
    }
#endif

    return nbrOverlap;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    // Extract line info

    list<Line> lines(0);

    int xmax = INT_MIN;
    int ymax = INT_MIN;

    for(auto s : data)
    {
        // cout << s << endl;
        int x1, y1, x2, y2;
        string s_scrap;
        char   c_scrap;

        std::stringstream ss(s);
        ss >> x1 >> c_scrap >> y1 >> s_scrap >> x2 >> c_scrap >> y2;

        xmax = std::max(xmax, std::max(x1, x2));
        ymax = std::max(ymax, std::max(y1, y2));

        Line l(x1, y1, x2, y2);

        lines.push_back(l);
    }

    // Force min coords at 0,0... Might not be optimal
    int map_deltax = xmax + 1;
    int map_deltay = ymax + 1;

    vector<int> map(map_deltax * map_deltay, 0);

    for(auto l : lines)
    {
        if(l.isHorizontal())
        {
            int xmin = std::min(l.p1.x, l.p2.x);
            int xmax = std::max(l.p1.x, l.p2.x);
            int y = l.p1.y;

            for(int x = xmin; x <= xmax; x++)
            {
                map[y*map_deltax + x] += 1;
            }

        }
        else if(l.isVertical())
        {
            int ymin = std::min(l.p1.y, l.p2.y);
            int ymax = std::max(l.p1.y, l.p2.y);
            int x = l.p1.x;

            for(int y = ymin; y <= ymax; y++)
            {
                map[y*map_deltax + x] += 1;
            }
        }
        else if(l.isDiagonal())
        {
            int l_deltax = (l.p2.x > l.p1.x) ? 1 : -1;
            int l_deltay = (l.p2.y > l.p1.y) ? 1 : -1;
            int nPoints = std::abs(l.p2.x - l.p1.x) + 1;

            int x = l.p1.x;
            int y = l.p1.y;

            for(int i = 0; i<nPoints; i++)
            {
                map[y*map_deltax + x] += 1;
                x += l_deltax;
                y += l_deltay;
            }
        }
    }

    int nbrOverlap = 0;

    for(auto m : map)
    {
        if(m >= 2)
            nbrOverlap++;
    }

#if 0
    // Show the map
    int counter = 1;
    for(auto m : map)
    {
        cout << m << " ";

        if(counter++ % (map_deltax) == 0)
        {
            cout << endl;
        }
    }
#endif

    return nbrOverlap;
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
        "0,9 -> 5,9",
        "8,0 -> 0,8",
        "9,4 -> 3,4",
        "2,2 -> 2,1",
        "7,0 -> 7,4",
        "6,4 -> 2,0",
        "0,9 -> 2,9",
        "3,4 -> 1,4",
        "0,0 -> 8,8",
        "5,5 -> 8,2"
    };

    assert(solve_puzzle1<vector<string>>(example1) == 5 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example1) == 12 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
