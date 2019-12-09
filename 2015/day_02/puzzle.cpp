//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"
#include <sstream>      // std::istringstream

using namespace std;

struct box
{
    int x;
    int y;
    int z;

    const int surface() const
    {
        return 2*x*y + 2*y*z + 2*x*z;
    }

    const int minSideSurface() const
    {
        return std::min(std::min(x*y, y*z), x*z);
    }

    const int maxSidePerimeter() const
    {
        return std::max(std::max(x, y), z);
    }

    const int totalSidePerimeter() const
    {
        return 2*x + 2*y + 2*z;
    }

    const int volume() const
    {
        return x*y*z;
    }
};

std::ostream& operator<<(std::ostream& os, const box& b)
{
    return os << b.x << "x" << b.y << "x" << b.z << " : " << b.surface() << " : "  << b.minSideSurface();
}


// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data)
{
    int totalArea = 0;
    for(string d : data)
    {
        std::istringstream iss (d);

        box b;
        char sep;

        iss >> b.x >> sep >> b.y >> sep >> b.z;

        // cout << b << endl;
        totalArea += b.surface() + b.minSideSurface();
    }

    return totalArea;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2(T data)
{
    int totalLength = 0;
    for(string d : data)
    {
        std::istringstream iss (d);

        box b;
        char sep;

        iss >> b.x >> sep >> b.y >> sep >> b.z;

        // cout << b << endl;

        totalLength += b.totalSidePerimeter()
            - 2*b.maxSidePerimeter()
            + b.volume();
    }

    return totalLength;
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

#if 0
    for(string d : data)
        cout << d << endl;
    cout << endl;
#endif

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const auto example1 = 1;
    assert(solve_puzzle1(data) == 1588178 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(data) == 3783758 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
