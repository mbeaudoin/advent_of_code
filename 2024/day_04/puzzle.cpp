//
// Copyright (C) Martin Beaudoin. 2024. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <numeric>
#include <algorithm>
#include <regex>
#include "myutils.h"

using namespace std;


struct Coord
{
    int x;
    int y;
};


template <typename T>
class patternMatch
{
    // Generate all possible matrix patterns for a ref pattern of a given size
    //
    // Here for a 4-letter pattern like XMAS
    //
    //        --------->    X axis
    //        |
    //        |
    //        |
    //        |
    //        V  Y axis
    //
    //         pattern 0:  ....  ->      stepX: [0, 1, 2, 3]
    //                                   stepY: [0, 0, 0, 0]
    //
    //         pattern 1:  ....  <-      stepX: [0, -1, -2, -3]
    //                                   stepY: [0,  0,  0,  0]
    //                     .
    //                     .
    //                     .   ^
    //         pattern 2:  .   |         stepX: [0,  0,  0,  0]
    //                                   stepY: [0, -1, -2, -3]
    //                     .
    //                     .
    //                     .   |
    //         pattern 3:  .   V         stepX: [0, 0, 0, 0]
    //                                   stepY: [0, 1, 2, 3]
    //
    //                        .
    //                       .
    //                      .  ^
    //         pattern 4:  .   /         stepX: [0,  1,  2,  3]
    //                                   stepY: [0, -1, -2, -3]
    //
    //                        .
    //                       .
    //                      .  /
    //         pattern 5:  .   V         stepX: [0, -1, -2, -3]
    //                                   stepY: [0,  1,  2,  3]
    //
    //                     .
    //                      .
    //                       .  ^
    //         pattern 6:     .  \       stepX: [0, -1, -2, -3]
    //                                   stepY: [0, -1, -2, -3]
    //
    //                     .
    //                      .
    //                       .  \
    //         pattern 7:     .  V       stepX: [0, 1, 2, 3]
    //                                   stepY: [0, 1, 2, 3]

private:
    int    _maxX;
    int    _maxY;
    string _pattern;

public:

    const struct Coord steps[8][4] = {
        {{ 0,  0}, { 1,  0}, { 2,  0}, { 3,  0}},  // pattern 0
        {{ 0,  0}, {-1,  0}, {-2,  0}, {-3,  0}},  // pattern 1
        {{ 0,  0}, { 0, -1}, { 0, -2}, { 0, -3}},  // pattern 2
        {{ 0,  0}, { 0,  1}, { 0,  2}, { 0,  3}},  // pattern 3
        {{ 0,  0}, { 1, -1}, { 2, -2}, { 3, -3}},  // pattern 4
        {{ 0,  0}, {-1,  1}, {-2,  2}, {-3,  3}},  // pattern 5
        {{ 0,  0}, {-1, -1}, {-2, -2}, {-3, -3}},  // pattern 6
        {{ 0,  0}, { 1,  1}, { 2,  2}, { 3,  3}}   // pattern 7
    }
    ;

    // Constructor
    patternMatch ()
        :
        _maxX(-1),
        _maxY(-1),
        _pattern("")
    {};

    patternMatch(int maxX, int maxY, string pattern)
        :
        _maxX(maxX),
        _maxY(maxY),
        _pattern(pattern)
    {
    }

    // Copy constructor
    patternMatch(const patternMatch &pm)
    {
        this->_maxX = pm._maxX;
        this->_maxY = pm._maxY;
        this->_pattern = pm._pattern;
    }

    bool checkInBound(const struct Coord& c)
    {
        return
            (c.x >= 0) && (c.x < _maxX) &&
            (c.y >= 0) && (c.y < _maxY);
    }

    int findMatch(int x, int y, T& data, bool debug=false)
    {
        int nbrMatch = 0;

        for (auto s : steps)
        {
            struct Coord letterCoords[4] = {
                {x + s[0].x, y + s[0].y},
                {x + s[1].x, y + s[1].y},
                {x + s[2].x, y + s[2].y},
                {x + s[3].x, y + s[3].y}
            };

            if (
                checkInBound(letterCoords[0]) &&
                checkInBound(letterCoords[1]) &&
                checkInBound(letterCoords[2]) &&
                checkInBound(letterCoords[3])
            )
            {
                char word[5] = {
                    data[letterCoords[0].y][letterCoords[0].x],
                    data[letterCoords[1].y][letterCoords[1].x],
                    data[letterCoords[2].y][letterCoords[2].x],
                    data[letterCoords[3].y][letterCoords[3].x],
                    '\0'
                };

                if(debug)
                    cout << "word: " << word << endl;

                if(std::strncmp(word, "XMAS", 4) == 0)
                {
                    nbrMatch++;
                }

            }
        }

        return nbrMatch;
    }

    friend ostream& operator<<(ostream& os, const patternMatch& line);

};


// Solve puzzle #1
template <typename T>
long long solve_puzzle1(T data, bool debug=false)
{
    if(debug)
    {
        for(auto s : data)
            cout<< s <<"\n";
    }

    int maxX = data[0].size();
    int maxY = data.size();

    int nbrMatch = 0;

    // Create the pattern matcher
    patternMatch<T> pm(maxX, maxY, "XMAS");

    for(int j=0; j<maxY; j++)
    {
        for(int i=0; i<maxX; i++)
        {
            nbrMatch += pm.findMatch(i, j, data, debug);
        }
    }

    return nbrMatch;
}


// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data, bool debug = false)
{
    int maxX = data[0].size();
    int maxY = data.size();

    int nbrMatch = 0;

    // Locate the A's. Check diagonal neighbours, quick and dirty
    for(int y=1; y<maxY-1; y++)      // Skip the upper and lower borders
    {
        for(int x=1; x<maxX-1; x++)  // Skip the left and right sides
        {
            if(data[y][x] == 'A')
            {
                char ur = data[y-1][x+1];  // upper right character
                char ul = data[y-1][x-1];  // upper left character
                char lr = data[y+1][x+1];  // lower right character
                char ll = data[y+1][x-1];  // lower left character

                if(
                    ((ur == 'M' && ll == 'S') || (ur == 'S' && ll == 'M')) &&
                    ((ul == 'M' && lr == 'S') || (ul == 'S' && lr == 'M'))
                )
                {
                    nbrMatch++;
                }
            }
        }
    }

    return nbrMatch;

    return 0;
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
    std::vector<string> data;
    bool keepSpace = true;
    bool keepEmptyLine = false;
    bool debug = false;

    myutils::read_file(data, filename, keepSpace, keepEmptyLine, debug);

    const std::vector<string> example1 = {
        "MMMSXXMASM",
        "MSAMXMSMSA",
        "AMXSXMAAMM",
        "MSAMASMSMX",
        "XMASAMXAMM",
        "XXAMMXXAMA",
        "SMSMSASXSS",
        "SAXAMASAAA",
        "MAMMMXMMMM",
        "MXMXAXMASX"
    };

    const std::vector<string> example2 = {
        ".M.S......",
        "..A..MSMS.",
        ".M.S.MAA..",
        "..A.ASMSM.",
        ".M.S.M....",
        "..........",
        "S.S.S.S.S.",
        ".A.A.A.A..",
        "M.M.M.M.M.",
        ".........."
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example1) == 18 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 2370

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example2) == 9 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 1908
}
