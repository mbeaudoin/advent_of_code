//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <valarray>
#include "myutils.h"

using namespace std;

struct houseCoord
{
    int x;
    int y;

    houseCoord& operator+=(const houseCoord& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
};

std::ostream& operator<<(std::ostream& os, const houseCoord& h)
{
    return os << h.x << ":" << h.y;
}

void
computePath(valarray<char> path, map<string, int>& visitedHouses)
{
    houseCoord curHouse({0, 0});
    string key = "00";
    visitedHouses[key]++;

    for(auto p : path)
    {
        switch(p)
        {
            case '^':
            {
                curHouse += {0, 1};
            }
            break;
            case 'v':
            {
                curHouse += {0, -1};
            }
            break;
            case '>':
            {
                curHouse += {1, 0};
            }
            break;
            case '<':
            {
                curHouse += {-1, 0};
            }
            break;
        }
        key = to_string(curHouse.x) + to_string(curHouse.y);

        visitedHouses[key]++;
    }

}

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data)
{
    houseCoord curHouse({0, 0});
    map<string, int> visitedHouses;

    string key = "00";

    visitedHouses[key]++;

    for(auto d : data)
    {
        switch(d)
        {
            case '^':
                curHouse += {0, 1};
                break;
            case 'v':
                curHouse += {0, -1};
                break;
            case '>':
                curHouse += {1, 0};
                break;
            case '<':
                curHouse += {-1, 0};
                break;
        }

        key = to_string(curHouse.x) + to_string(curHouse.y);

        visitedHouses[key]++;
    }

    return visitedHouses.size();
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2(T data)
{
    valarray<char> fullPath(data.size());
    for(int i=0; i<data.size(); i++)
        fullPath[i] = data[i];

    // use valarray and slice with strides to extract Santa and Robot paths
    valarray<char> santaPath = fullPath[std::slice(0,data.size()/2, 2)];
    valarray<char> robotPath = fullPath[std::slice(1,data.size()/2, 2)];

#if 0
    cout << "size of fullPath:" << fullPath.size() << endl;
    cout << "size of santaPath:" << santaPath.size() << endl;
    cout << "size of robotPath:" << robotPath.size() << endl;

    cout << "santaPath: " << endl;
    for(auto p : santaPath)
        cout << p;
    cout << endl;

    cout << "robotPath: " << endl;
    for(auto p : robotPath)
        cout << p;
    cout << endl;
#endif

    map<string, int> visitedHouses;

    computePath(santaPath, visitedHouses);
    computePath(robotPath, visitedHouses);

    return visitedHouses.size();
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
    assert(solve_puzzle1(data[0]) == 2592 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data[0]) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(data[0]) == 2360 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data[0]) << std::endl;
}
