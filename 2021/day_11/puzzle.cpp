//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>      // std::stringstream
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, int maxNbSteps, int& stepFullKaboom, int debug = 0)
{
    const int MAX_X = 10;
    const int MAX_Y = 10;

    int octoField[MAX_X][MAX_Y];

    // lambda function : print octo field
    auto dumpField = [MAX_X, MAX_Y, &octoField]() ->void {
        for(int j=0; j<MAX_Y; j++)
        {
            for(int i=0; i<MAX_X; i++)
            {
                cout << octoField[i][j];
            }
            cout << endl;
        }
        cout << endl;
    };

    auto validCoords = [MAX_X, MAX_Y](int x, int y) -> bool {
        return (
            x >= 0 &&
            y >= 0 &&
            x < MAX_X &&
            y < MAX_Y
        );
    };

    // Extract map of octopus
    int j = 0;
    for(auto d : data)
    {
        int i=0;
        for(auto c: d)
        {
            octoField[i++][j] = c - '0';
        }
        j++;
    }

    if(debug)
        dumpField();

     // lambda function : increase energy
    auto increaseEnergy = [MAX_X, MAX_Y, &octoField]() ->void {
        for(int j=0; j<MAX_Y; j++)
        {
            for(int i=0; i<MAX_X; i++)
            {
                octoField[i][j]++;
            }
        }
    };

    // recursive lambda function : do the flashes
    std::function<long(int, int)> flashOcto;
    flashOcto = [
        &octoField,
        &validCoords,
        &flashOcto](int x, int y) ->long
        {
            long nbrFlashes = 0;
            int neighboorsOffset[8][2] = {
                {-1, 0},
                { 1, 0},
                { 0,-1},
                { 0, 1},
                {-1,-1},
                {-1, 1},
                { 1,-1},
                { 1, 1}
            };

            // Skip blown octopuses
            if(octoField[x][y] != 0)
            {
                if(octoField[x][y] > 9)
                {
                    octoField[x][y] = 0;
                    nbrFlashes++;

                    // First, boost neighbours power
                    for(auto n : neighboorsOffset)
                    {
                        int new_x = x + n[0];
                        int new_y = y + n[1];

                        if(validCoords(new_x, new_y) && octoField[new_x][new_y] != 0)
                        {
                            octoField[new_x][new_y]++;
                        }
                    }

                    // Then blow up neighbours
                    for(auto n : neighboorsOffset)
                    {
                        int new_x = x + n[0];
                        int new_y = y + n[1];

                        if(validCoords(new_x, new_y))
                        {
                            nbrFlashes += flashOcto(new_x, new_y);
                        }
                    }
                }
            }
            return nbrFlashes;
        };

    auto doFlashes = [
        &octoField,
        &flashOcto]() ->long {

        long nbrFlashes = 0;
        for(int j=0; j<MAX_Y; j++)
        {
            for(int i=0; i<MAX_X; i++)
            {
                nbrFlashes += flashOcto(i, j);
            }
        }
        return nbrFlashes;
    };

    long nbrFlashes = 0;
    for(int step = 1; step <= maxNbSteps; step++)
    {
        increaseEnergy();

        nbrFlashes += doFlashes();

        if(debug)
        {
            cout << "Step " << step << ":\n";
            dumpField();
            cout << "Nbr flashes: "  << nbrFlashes << endl;
        }

        // Check for full 1st kaboom
        if(stepFullKaboom == 0)
        {
            int sumEnergyLevel = 0;
            for(int j=0; j<MAX_Y; j++)
            {
                for(int i=0; i<MAX_X; i++)
                {
                    sumEnergyLevel += octoField[i][j];
                }
            }

            // Only memorize first full kaboom step
            if(sumEnergyLevel == 0)
                stepFullKaboom = step;
        }
    }

    return nbrFlashes;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    int stepFullKaboom = 0;

    // 1000 steps should be plenty
    solve_puzzle1(data, 1000, stepFullKaboom);

    return stepFullKaboom;
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
        "5483143223",
        "2745854711",
        "5264556173",
        "6141336146",
        "6357385478",
        "4167524645",
        "2176841721",
        "6882881134",
        "4846848554",
        "5283751526"
    };
    int stepFullKaboom = 0;

    assert(solve_puzzle1<vector<string>>(example1, 100, stepFullKaboom) == 1656 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 100, stepFullKaboom) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example1) == 195 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
