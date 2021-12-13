//
// Copyright (C) Martin Beaudoin. 2021. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>      // std::stringstream
#include <climits>
#include <cstring>
#include <numeric>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, int debug=0)
{
    vector<myutils::point> coords;

    vector<pair<char, int>> folds;

    int minCoord_x =  INT_MAX;
    int maxCoord_x =  INT_MIN;
    int minCoord_y =  INT_MAX;
    int maxCoord_y =  INT_MIN;

    for(auto d : data)
    {
        if(debug)
            cout << d << endl;

        // Extract data; find min/max coords along the way
        std::stringstream ss(d);

        if(d[0] == 'f')
        {
            pair<char, int> fold;
            string token;
            std::getline(ss, token, '=');
            fold.first = token[token.size() - 1];

            std::getline(ss, token, '=');
            fold.second = stoi(token);
            folds.push_back(fold);
        }
        else
        {
            myutils::point coord;
            char sep;
            ss >> coord.x >> sep >> coord.y;
            coords.push_back(coord);

            minCoord_x = std::min(minCoord_x, coord.x);
            minCoord_y = std::min(minCoord_y, coord.y);
            maxCoord_x = std::max(maxCoord_x, coord.x);
            maxCoord_y = std::max(maxCoord_y, coord.y);
        }
    }

    if(debug)
    {
        cout << "Coords: " << endl;
        for(auto c : coords)
            cout << c << endl;

        cout << "Folds: " << endl;
        for(auto f : folds)
            cout << f.first << " along " << f.second << endl;

        cout << "minCoord_x: " << minCoord_x << endl;
        cout << "minCoord_y: " << minCoord_y << endl;
        cout << "maxCoord_x: " << maxCoord_x << endl;
        cout << "maxCoord_y: " << maxCoord_y << endl;
    }

    int nbrDots = 0;
    int sizeX = maxCoord_x - minCoord_x + 1;
    int sizeY = maxCoord_y - minCoord_y + 1;

    vector<vector<int>>paper;

    // Write paper dots on console
    auto dumpPaper = [&paper]() ->void {
        int iMax = paper.size();
        int jMax = paper[0].size();

        for(int j=0; j<jMax; j++)
        {
            for(int i=0; i<iMax; i++)
            {
                cout << (paper[i][j] == 1 ? '#' : ' ');
            }
            cout << endl;
        }
        cout << endl;
    };

    // Initialize
    paper.resize(sizeX);  // Nbr columns
    for(auto& v : paper)
        v.resize(sizeY, 0); // Resize each column

    // Insert problem data
    for(auto c : coords)
    {
        paper[c.x][c.y] = 1;
    }

    // Starting configuration
    if(debug)
        dumpPaper();

    // Start folding
    for(auto f : folds)
    {
        char foldDir = f.first;
        int  foldIndex = f.second;

        sizeX = paper.size();
        sizeY = paper[0].size();

        cout << "Folding along: " << foldDir << " at : "  << foldIndex << endl;
        switch(foldDir)
        {
            case 'y':
                for(int j=foldIndex; j<sizeY; j++)
                {
                    for(int i=0; i<sizeX; i++)
                    {
                        paper[i][sizeY-j-1] |= paper[i][j];
                        paper[i][j] = 0;
                    }
                }

                // Cleanup: remove folded section of page
                for(auto &v : paper)
                    v.resize(foldIndex);

                break;

            case 'x':
                for(int j=0; j<sizeY; j++)
                {
                    for(int i=foldIndex; i<sizeX; i++)
                    {
                        paper[sizeX-i-1][j] |= paper[i][j];
                        paper[i][j] = 0;
                    }
                }

                // Cleanup: remove folded section of page
                paper.resize(foldIndex);

                break;
        }

        if(debug)
            dumpPaper();

        // Count dots
        int nDots = 0;
        for(auto &v : paper)
            nDots = std::accumulate(v.begin(), v.end(), nDots);

        if(nbrDots == 0)
            nbrDots = nDots;
    }
    // Show results of last fold
    dumpPaper();

    return nbrDots;
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
    const vector<string> example1 = {
        "6,10",
        "0,14",
        "9,10",
        "0,3",
        "10,4",
        "4,11",
        "6,0",
        "6,12",
        "4,1",
        "0,13",
        "10,12",
        "3,4",
        "3,0",
        "8,4",
        "1,10",
        "2,14",
        "8,10",
        "9,0",
        "fold along y=7",
        "fold along x=5"
    };
    assert(solve_puzzle1<vector<string>>(example1) == 17 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1 and #2: "<< solve_puzzle1(data) << " <<-- Part #1" << std::endl;
}
