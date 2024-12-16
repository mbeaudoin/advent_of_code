//
// Copyright (C) Martin Beaudoin. 2024. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <map>
#include <numeric>
#include <algorithm>
#include "myutils.h"

using namespace std;

template <typename T>
struct navMap {

    typedef std::pair<int, int> Pos;

    enum Direction
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    std::map<Direction, char> DirectionToString =
    {
        { NORTH, 'N' },
        { EAST,  'E' },
        { SOUTH, 'S' },
        { WEST,  'W' }
    };

    const Pos _goNorth{ 0, -1};
    const Pos _goEast { 1,  0};
    const Pos _goSouth{ 0,  1};
    const Pos _goWest {-1,  0};

    // Map info
    T _map;     // input data
    T _mapDir;  // shadow map for detecting path loops

    int _max_x;
    int _max_y;

    // Travel info
    Pos _curPos;
    Pos _startingPosition;

    enum Direction      _curDir;

    bool _debug;

    // Constructor
    navMap (T theMap, bool debug=false) :
        _map(theMap),
        _mapDir(theMap),
        _max_y(_map.size()),
        _max_x(_map[0].size()),
        _debug(debug)
    {
        // Initialize the starting point
        for(int i=0; i<_max_y; i++)
        {
            std::string::size_type pos = _map[i].find("^");
            if (pos != std::string::npos)
            {
                _curPos.first = pos;
                _curPos.second = i;
                _curDir = NORTH;
                _startingPosition = _curPos;

                if(_debug)
                    cout << "Starting point: x: " << _curPos.second << " : y: " <<  _curPos.first << endl;

                break;
            }
        }
    }

    // turn right 90 degrees
    void changeDirection()
    {
        switch(_curDir)
        {
            case NORTH:
                _curDir = EAST;
                break;
            case EAST:
                _curDir = SOUTH;
                break;
            case SOUTH:
                _curDir = WEST;
                break;
            case WEST:
                _curDir = NORTH;
                break;
        }
    }

    bool detectObstacle()
    {
        Pos obstaclePos = _curPos;

        switch(_curDir)
        {
            case NORTH:
                obstaclePos.first  += _goNorth.first;
                obstaclePos.second += _goNorth.second;
                break;
            case EAST:
                obstaclePos.first  += _goEast.first;
                obstaclePos.second += _goEast.second;
                break;
            case SOUTH:
                obstaclePos.first  += _goSouth.first;
                obstaclePos.second += _goSouth.second;
                break;
            case WEST:
                obstaclePos.first  += _goWest.first;
                obstaclePos.second += _goWest.second;
                break;
        }

        // First, is the obstacle out of bound??
        bool obsOutOfBound =
            obstaclePos.first  < 0       ||
            obstaclePos.second < 0       ||
            obstaclePos.first  >= _max_y ||
            obstaclePos.second >= _max_x;


        return
            !obsOutOfBound &&
            (
                _map[obstaclePos.second][obstaclePos.first] == '#' ||
                _map[obstaclePos.second][obstaclePos.first] == 'O'
            );

    }

    void markLocation()
    {
        _map   [_curPos.second][_curPos.first] = 'X';
    }

    void markDirection()
    {
        _mapDir[_curPos.second][_curPos.first] = DirectionToString[_curDir];
    }

    bool outOfBound()
    {
        return
            _curPos.first  < 0       ||
            _curPos.second < 0       ||
            _curPos.first  >= _max_y ||
            _curPos.second >= _max_x;
    }

    void addObstacle(int x, int y)
    {
        if
        (
            // Don't put obstacle at guard location
            _map[y][x] != '^'
        )
        {
            _map[y][x] = 'O';
        }
    }

    bool isLooping()
    {
        // Is the current next step already visited from the
        // same direction??
        return
        (
            _map   [_curPos.second][_curPos.first] == 'X' &&
            _mapDir[_curPos.second][_curPos.first] == DirectionToString[_curDir]
        );
    }

    // Navigate the map. At the end, report the number of distinct positions
    int navigate(bool& loopDetected)
    {
        int nbrPosVisited = 0;

        loopDetected = false;

        while(! outOfBound())
        {
            // Bumping into something?
            while( detectObstacle() )
            {
                changeDirection();
            }

            // Detect loops
            if (isLooping ())
            {
                loopDetected = true;

                break;
            }

            markLocation();
            markDirection();

            if(_debug)
                cout << "marked cur_pos: " << _curPos.second << ", " << _curPos.first
                    << endl << *this << endl;

            // Next step, according to curDir
            switch(_curDir)
            {
                case NORTH:
                    _curPos.first  += _goNorth.first;
                    _curPos.second += _goNorth.second;
                    break;
                case EAST:
                    _curPos.first  += _goEast.first;
                    _curPos.second += _goEast.second;
                    break;
                case SOUTH:
                    _curPos.first  += _goSouth.first;
                    _curPos.second += _goSouth.second;
                    break;
                case WEST:
                    _curPos.first  += _goWest.first;
                    _curPos.second += _goWest.second;
                    break;
            }
        }

        // We are done
        if(! loopDetected)
        {
            if(_debug)
                cout << "Out of bound: " << endl
                    << *this << endl;

            // Compute number of marks left
            for(int i=0; i<_max_y; i++)
            {
                for(int j=0; j<_max_y; j++)
                {
                    if(_map[i][j] == 'X')
                    {
                        nbrPosVisited++;
                    }
                }
            }
        }

        return nbrPosVisited;
    }

    // Output stream
    friend std::ostream &operator<<( std::ostream &os, const navMap & rhs)
    {
        int i=0;

        for(auto l : rhs._map)
        {
            // os << l << " " << rhs._mapDir[i++] << endl;
            os << l << endl;
        }
        return os;
    }
};

// Solve puzzle #1
template <typename T>
long long solve_puzzle1(T data, bool debug=false)
{
    if(debug)
    {
        for(auto s : data)
            std::cout<< s <<"\n";
    }

    struct navMap<T> theMap(data, debug);

    bool loopDetected = false;

    int nDistinctPos = theMap.navigate(loopDetected);

    return nDistinctPos;
}


// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data, bool debug = false)
{
    if(debug)
    {
        for(auto s : data)
            std::cout<< s <<"\n";
    }

    bool loopDetected = false;

    long nbrStuckPos = 0;

    struct navMap<T> origMap(data);

    for(int y=0; y< origMap._max_y; y++)
    {
        for(int x=0; x< origMap._max_x; x++)
        {
            // Start from a fresh new map
            struct navMap<T> theMap(data, debug);

            // Add obstacle
            theMap.addObstacle(x, y);

            if(debug)
                cout << "Adding obstacle at: "
                    "x: " << x << " y: " << y << " : " << data[y]
                    << endl << theMap << endl;

            int nDistinctPos = theMap.navigate(loopDetected);

            if(loopDetected)
            {
                if(debug)
                    cout << "Loop detected with obstacle at: " <<
                        " x: " << x <<
                        " : y: " << y << endl;

                nbrStuckPos++;
            }
        }
    }

    return nbrStuckPos;
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

    const std::vector<string> example = {
        "....#.....",
        ".........#",
        "..........",
        "..#.......",
        ".......#..",
        "..........",
        ".#..^.....",
        "........#.",
        "#.........",
        "......#..."
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 41 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 5534

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 6 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 2262
}
