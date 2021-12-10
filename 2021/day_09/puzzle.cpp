//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <sstream>      // std::stringstream
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, vector<myutils::point>& lowPoints, int debug=0)
{
    // Convert data to heights
    int mapWidth = data[0].size();
    int mapHeight = data.size();

    // lambda function : 2D coords to index
    auto coordsToIndex = [mapWidth](int x, int y) ->int {
        return x + y*mapWidth;
    };

    vector<int> heightmap(0);

    for(auto d: data)
    {
        for(auto c: d)
        {
            heightmap.push_back(c - '0');
        }
    }

    if(debug > 0)
    {
        int counter = 0;
        for(auto h : heightmap)
        {
            cout << h;
            if(++counter % mapWidth == 0)
                cout << endl;
        }
    }

    int sumRiskLevel = 0;

    // Find low points
    // Corners
    if(
        heightmap[coordsToIndex(0, 0)] < heightmap[coordsToIndex(1, 0)] &&
        heightmap[coordsToIndex(0, 0)] < heightmap[coordsToIndex(0, 1)]
        )
    {
        sumRiskLevel += heightmap[coordsToIndex(0, 0)] + 1;
        lowPoints.push_back(myutils::point(0, 0));

        if(debug >= 1)
            cout << "Debug1: Found low " << heightmap[coordsToIndex(0, 0)] << " at 0,0 " << " : " << sumRiskLevel << endl;
    }

    if(
        heightmap[coordsToIndex(mapWidth-1, 0)] < heightmap[coordsToIndex(mapWidth-2, 0)] &&
        heightmap[coordsToIndex(mapWidth-1, 0)] < heightmap[coordsToIndex(mapWidth-1, 1)]
        )
    {
        sumRiskLevel += heightmap[coordsToIndex(mapWidth-1, 0)] + 1;
        lowPoints.push_back(myutils::point(mapWidth-1, 0));

        if(debug >= 2)
            cout << "Debug2: Found low " << heightmap[coordsToIndex(mapWidth-1, 0)] << " at " << mapWidth-1 << "," << 0 << " : " << sumRiskLevel << endl;
    }

    if(
        heightmap[coordsToIndex(0, mapHeight-1)] < heightmap[coordsToIndex(1, mapHeight-1)] &&
        heightmap[coordsToIndex(0, mapHeight-1)] < heightmap[coordsToIndex(0, mapHeight-2)]
        )
    {
        sumRiskLevel += heightmap[coordsToIndex(0, mapHeight-1)] + 1;
        lowPoints.push_back(myutils::point(0, mapHeight-1));

        if(debug == 3)
            cout << "Debug3: Found low " << heightmap[coordsToIndex(0, mapHeight-1)] << " at " << 0 << "," << mapHeight-1 << " : " << sumRiskLevel << endl;
    }

    if(
        heightmap[coordsToIndex(mapWidth-1, mapHeight-1)] < heightmap[coordsToIndex(mapWidth-2, mapHeight-1)] &&
        heightmap[coordsToIndex(mapWidth-1, mapHeight-1)] < heightmap[coordsToIndex(mapWidth-1, mapHeight-2)]
        )
    {
        sumRiskLevel += heightmap[coordsToIndex(mapWidth-1, mapHeight-1)] + 1;
        lowPoints.push_back(myutils::point(mapWidth-1, mapHeight-1));

        if(debug >= 4)
            cout << "Debug4: Found low " << heightmap[coordsToIndex(mapWidth-1, mapHeight-1)] << " at " << mapWidth-1 << "," << mapHeight-1 << " : " << sumRiskLevel << endl;
    }



    // Top and Bottom rows
    for(int i=1; i<mapWidth-1; i++)
    {
        if(
            heightmap[coordsToIndex(i, 0)] < heightmap[coordsToIndex(i-1, 0)] &&
            heightmap[coordsToIndex(i, 0)] < heightmap[coordsToIndex(i+1, 0)] &&
            heightmap[coordsToIndex(i, 0)] < heightmap[coordsToIndex(i  , 1)]
        )
        {
            sumRiskLevel += heightmap[coordsToIndex(i, 0)] + 1;
            lowPoints.push_back(myutils::point(i, 0));

            if(debug >= 5)
                cout << "Debug5: Found low " << heightmap[coordsToIndex(i, 0)] << " at " << i << "," << 0 << " : " << sumRiskLevel << endl;
        }

        if(
            heightmap[coordsToIndex(i, mapHeight-1)] < heightmap[coordsToIndex(i-1, mapHeight-1)] &&
            heightmap[coordsToIndex(i, mapHeight-1)] < heightmap[coordsToIndex(i+1, mapHeight-1)] &&
            heightmap[coordsToIndex(i, mapHeight-1)] < heightmap[coordsToIndex(i  , mapHeight-2)]
        )
        {
            sumRiskLevel += heightmap[coordsToIndex(i, mapHeight-1)] + 1;
            lowPoints.push_back(myutils::point(i, mapHeight-1));

            if(debug >= 6)
                cout << "Debug6: Found low " << heightmap[coordsToIndex(i, mapHeight-1)] << " at " << i << "," << mapHeight-1 << " : " << sumRiskLevel << endl;
        }


    }

    // Left and Right columns
    for(int i=1; i<mapHeight-1; i++)
    {
        if(
            heightmap[coordsToIndex(0, i)] < heightmap[coordsToIndex(0, i-1)] &&
            heightmap[coordsToIndex(0, i)] < heightmap[coordsToIndex(0, i+1)] &&
            heightmap[coordsToIndex(0, i)] < heightmap[coordsToIndex(1, i)]
        )
        {
            sumRiskLevel += heightmap[coordsToIndex(0, i)] + 1;
            lowPoints.push_back(myutils::point(0, i));

            if(debug >= 7)
                cout << "Debug7: Found low " << heightmap[coordsToIndex(0, i)] << " at " << 0 << "," << i << " : " << sumRiskLevel << endl;
        }

        if(
            heightmap[coordsToIndex(mapWidth-1, i)] < heightmap[coordsToIndex(mapWidth-1, i-1)] &&
            heightmap[coordsToIndex(mapWidth-1, i)] < heightmap[coordsToIndex(mapWidth-1, i+1)] &&
            heightmap[coordsToIndex(mapWidth-1, i)] < heightmap[coordsToIndex(mapWidth-2, i)]
        )
        {
            sumRiskLevel += heightmap[coordsToIndex(mapWidth-1, i)] + 1;
            lowPoints.push_back(myutils::point(mapWidth-1, i));

            if(debug >= 8)
                cout << "Debug8: Found low " << heightmap[coordsToIndex(mapWidth-1, i)] << " at " << mapWidth-1 << "," << i << " : " << sumRiskLevel << endl;
        }
    }

    // All the rest
    for(int j=1; j<mapHeight-1; j++)
    {
            for(int i=1; i<mapWidth-1; i++)
            {
                if(
                    heightmap[coordsToIndex(i, j)] < heightmap[coordsToIndex(i-1, j  )] &&
                    heightmap[coordsToIndex(i, j)] < heightmap[coordsToIndex(i+1, j  )] &&
                    heightmap[coordsToIndex(i, j)] < heightmap[coordsToIndex(i  , j-1)] &&
                    heightmap[coordsToIndex(i, j)] < heightmap[coordsToIndex(i  , j+1)]
                )
                {
                    sumRiskLevel += heightmap[coordsToIndex(i, j)] + 1;
                    lowPoints.push_back(myutils::point(i, j));

                    if(debug >= 9)
                        cout << "Debug9: Found low " << heightmap[coordsToIndex(i, j)] << " at " << i << "," << j << " : " << sumRiskLevel << endl;
                }
            }
    }

    if(debug > 0)
        cout << "Risk: "  << sumRiskLevel << endl;

    return sumRiskLevel;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, vector<myutils::point>& lowPointsCoords, int debug=0)
{
    // Convert data to heights
    int mapWidth = data[0].size();
    int mapHeight = data.size();

    // lambda function : 2D coords to index
    auto coordsToIndex = [mapWidth](int x, int y) ->int {
        return x + y*mapWidth;
    };

    // Convert data into height map
    vector<int> heightmap(0);

    for(auto d: data)
    {
        for(auto c: d)
        {
            heightmap.push_back(c - '0');
        }
    }

    if(debug > 0)
    {
        int counter = 0;
        for(auto h : heightmap)
        {
            cout << h;
            if(++counter % mapWidth == 0)
                cout << endl;
        }

        for(auto c: lowPointsCoords)
            cout << c << endl;
    }

    // Areas
    vector<int> areaBasins(0);

    for(auto c: lowPointsCoords)
    {
        set<int> basinCoords;
        basinCoords.insert(coordsToIndex(c.x, c.y));

        // Recursive lambda function to identify all neighbours from low point
        std::function<int(int,int)> checkForBasin;
        checkForBasin = [
            heightmap,
            mapWidth,
            mapHeight,
            debug,
            coordsToIndex,
            &basinCoords,
            &checkForBasin](int x, int y)->int
        {
            if(debug > 4)
            {
                cout << "Content of basinCoords: ";
                for(auto c : basinCoords)
                {
                    cout << c << " : ";
                }
                cout << endl;
            }

            int index = coordsToIndex(x, y);

            if(debug > 4)
            {
                cout << " Checking : " << x << "," << y << " : "
                    << (basinCoords.find(index) == basinCoords.end()) << " : "
                    << endl;
            }

            if(
                basinCoords.find(index) == basinCoords.end() &&
                x >= 0       &&
                x < mapWidth &&
                y >=0        &&
                y < mapHeight &&
                heightmap[index] != 9
            )
            {

                if(debug > 1)
                {
                    cout << " Adding: " << x << "," << y  << " : " << heightmap[index] << endl;
                }

                basinCoords.insert(index);  // Memorize

                checkForBasin(x+1, y+0); // Recursively check neighbours
                checkForBasin(x-1, y+0);
                checkForBasin(x+0, y+1);
                checkForBasin(x+0, y-1);
            }

            return 0;
        };

        // Recursively check neighbors for basin limits and recurse
        checkForBasin(c.x+1, c.y+0);
        checkForBasin(c.x-1, c.y+0);
        checkForBasin(c.x+0, c.y+1);
        checkForBasin(c.x+0, c.y-1);

        // Area is equal to total number of neighbours + low point
        areaBasins.push_back(basinCoords.size());

        if(debug > 4)
        {
            cout << "Low point: " << c << " : Basin Area: "  << basinCoords.size() << endl;
            for(auto c: basinCoords)
                cout << c << endl;
        }
    }

    // Sort basin areas in descending order
    sort(areaBasins.begin(), areaBasins.end(), greater <>());

    return areaBasins[0] * areaBasins[1] * areaBasins[2];
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
        "2199943210",
        "3987894921",
        "9856789892",
        "8767896789",
        "9899965678"
    };

    vector<myutils::point> lowPointsDemo(0);
    assert(solve_puzzle1<vector<string>>(example1, lowPointsDemo, 0) == 15 && "Error verifying puzzle #1");

    // Solve puzzle #1
    vector<myutils::point> lowPoints(0);
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, lowPoints, 0) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example1, lowPointsDemo) == 1134 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data, lowPoints) << std::endl;
}
