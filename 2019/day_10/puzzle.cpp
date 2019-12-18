//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include "myutils.h"
#include <cfloat>
#include <map>
#include <set>

using namespace std;

typedef myutils::point coords;

void
extractAsteroids(vector<string>& data, vector<coords>& asteroids)
{
    int x = 0;
    int y = 0;

    for(auto line : data)
    {
        x = 0;
        for(auto c : line)
        {
            if(c == '#')
            {
                coords coord(x, y);
                asteroids.push_back(coord);
            }
            x++;
        }
        y++;
    }

    // cout << "Nbr asteroids: " << asteroids.size() << endl;

    //for(auto a : asteroids)
    //    cout << a << endl;

    return;
}

int
computeVisibility(coords& ref, vector<coords>& asteroids)
{
    int deltax, deltay, dist2;
    double slope;

    vector<coords> pos_deltax, neg_deltax;
    vector<coords> pos_deltay, neg_deltay;
    map<double, coords> diag_delta;

    for(auto a : asteroids)
    {
        if(a != ref)
        {
            deltax = a.x - ref.x;
            deltay = a.y - ref.y;
            dist2 = deltax*deltax + deltay*deltay;

            if(abs(deltax) == 0)
            {
                // Vertical alignment
                if(deltay > 0)
                {
                    pos_deltay.push_back(a);
                }
                else
                {
                    neg_deltay.push_back(a);
                }
            }
            else if (abs(deltay) == 0)
            {
                // Horizontal alignment
                if(deltax > 0)
                {
                    pos_deltax.push_back(a);
                }
                else
                {
                    neg_deltax.push_back(a);
                }
            }
            else
            {
                // Diagonal alignment
                double quadrant_indicator;

                if(deltax > 0.0 && deltay > 0.0)
                    quadrant_indicator = 100.0;
                else if(deltax < 0.0 && deltay > 0.0)
                    quadrant_indicator = 200.0;
                else if(deltax < 0.0 && deltay < 0.0)
                    quadrant_indicator = 300.0;
                else
                    quadrant_indicator = 400.0;

                slope = double(deltay)/deltax + quadrant_indicator;
                diag_delta[slope] = a;

                //cout << ref << " : " << a << " : dist2: " << dist2 << " : slope: " << slope<< endl;
            }
        }
    }

    int direct_los =
        (pos_deltax.size() > 0 ? 1 : 0) +
        (pos_deltay.size() > 0 ? 1 : 0) +
        (neg_deltax.size() > 0 ? 1 : 0) +
        (neg_deltay.size() > 0 ? 1 : 0) +
        diag_delta.size();

    // cout << ref << " : direct los: " << direct_los << endl;

    return direct_los ;
}

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data, coords& bestLocation)
{
    //for(auto d : data)
    //    cout << d << endl;

    vector<coords> asteroids;
    extractAsteroids(data, asteroids);

    int max_visibility = 0;

    for(auto a : asteroids)
    {
        int nbr_visible = computeVisibility(a, asteroids);

        if(nbr_visible  > max_visibility)
        {
            max_visibility = nbr_visible;
            bestLocation = a;
        }
    }

    //cout << "max_visibility: " << max_visibility << " at location: " << bestLocation << endl;

    return max_visibility;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2(T data, coords& bestLocation, int targetToVaporize)
{
    vector<coords> asteroids;
    extractAsteroids(data, asteroids);

    // map sorted from greatest angle to smallest
    // set sorted from closest to farthest using the < operator
    map<double, set<coords>, std::greater<double> > clockwiseSortedAsteroids;

    // Compute angle between best location and all asteroids
    for(auto a : asteroids)
    {
        if(a != bestLocation)
        {
            //By flipping the y and x axis, we get angle values going from max pos to min neg clockwise
            coords delta = {a.y - bestLocation.y, a.x - bestLocation.x};

            double angle =  myutils::atan2InDeg(delta);
            clockwiseSortedAsteroids[angle].insert(delta);
        }
    }

    // Now iterate through the map, removing the closest available asteroids each time
    int nbrVaporize = 0;
    coords lastBlastedAsteroid;

    do
    {
        for (auto const& sortedAsteroids : clockwiseSortedAsteroids)
        {
            if(!sortedAsteroids.second.empty())
            {
                coords delta_blastedAsteroid = *(sortedAsteroids.second.begin());

                // Remove from set
                clockwiseSortedAsteroids[sortedAsteroids.first].erase(delta_blastedAsteroid);

                nbrVaporize++;

                lastBlastedAsteroid = bestLocation + coords(delta_blastedAsteroid.y, delta_blastedAsteroid.x);

                //cout << "blasted #: " << nbrVaporize << " : " <<  sortedAsteroids.first << " : " << lastBlastedAsteroid
                //    << " : remaining: " << clockwiseSortedAsteroids[sortedAsteroids.first].size() << endl;

                if(nbrVaporize == targetToVaporize)
                    break;
            }
        }
    } while( nbrVaporize < targetToVaporize);

    return lastBlastedAsteroid.x*100 + lastBlastedAsteroid.y;
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
    const vector<string> example1 =
    {
        ".#..#",
        ".....",
        "#####",
        "....#",
        "...##"
    };
    coords bestLocation1;
    assert(solve_puzzle1(example1, bestLocation1) == 8 && "Error verifying puzzle #1");

    const vector<string> example2 =
    {
        "......#.#.",
        "#..#.#....",
        "..#######.",
        ".#.#.###..",
        ".#..#.....",
        "..#....#.#",
        "#..#....#.",
        ".##.#..###",
        "##...#..#.",
        ".#....####"
    };
    coords bestLocation2;
    assert(solve_puzzle1(example2, bestLocation2) == 33 && "Error verifying puzzle #1");

    const vector<string> example3 =
    {
        "#.#...#.#.",
        ".###....#.",
        ".#....#...",
        "##.#.#.#.#",
        "....#.#.#.",
        ".##..###.#",
        "..#...##..",
        "..##....##",
        "......#...",
        ".####.###."
    };
    coords bestLocation3;
    assert(solve_puzzle1(example3, bestLocation3) == 35 && "Error verifying puzzle #1");

    const vector<string> example4 =
    {
        ".#..#..###",
        "####.###.#",
        "....###.#.",
        "..###.##.#",
        "##.##.#.#.",
        "....###..#",
        "..#.#..#.#",
        "#..#.#.###",
        ".##...##.#",
        ".....#.#.."
    };
    coords bestLocation4;
    assert(solve_puzzle1(example4, bestLocation4) == 41 && "Error verifying puzzle #1");

    const vector<string> example5 =
    {
        ".#..##.###...#######",
        "##.############..##.",
        ".#.######.########.#",
        ".###.#######.####.#.",
        "#####.##.#.##.###.##",
        "..#####..#.#########",
        "####################",
        "#.####....###.#.#.##",
        "##.#################",
        "#####.##.###..####..",
        "..######..##.#######",
        "####.##.####...##..#",
        ".#####..#.######.###",
        "##...#.##########...",
        "#.##########.#######",
        ".####.#.###.###.#.##",
        "....##.##.###..#####",
        ".#.#.###########.###",
        "#.#.#.#####.####.###",
        "###.##.####.##.#..##"
    };
    coords bestLocation5;
    assert(solve_puzzle1(example5, bestLocation5) == 210 && "Error verifying puzzle #1");

    // Solve puzzle #1
    coords bestLocation;
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, bestLocation) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(example5, bestLocation5,   1) == 1112 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5,   2) == 1201 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5,   3) == 1202 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5,  10) == 1208 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5,  20) == 1600 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5,  50) == 1609 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5, 100) == 1016 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5, 199) ==  906 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5, 200) ==  802 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5, 201) == 1009 && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5, bestLocation5, 299) == 1101 && "Error verifying puzzle #2");

    // Solve puzzle #2
    assert(solve_puzzle2(data, bestLocation, 200) == 512 && "Error verifying puzzle #2");
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data, bestLocation, 200) << std::endl;
}
