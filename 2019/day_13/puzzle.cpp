//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"
#include <map>

#include "Intcode.h"
#include <curses.h>
#include <chrono>
#include <thread>

using namespace std;

typedef enum
{
    TILE_EMPTY  = 0,
    TILE_WALL   = 1,
    TILE_BLOCK  = 2,
    TILE_PADDLE = 3,
    TILE_BALL   = 4
} tileType;

// For screen rendering
map<int, char> pixValue;

// Solve puzzle #1
template <typename T>
long long solve_puzzle1(T data, int& xmin, int& xmax, int& ymin, int& ymax, bool debug = false)
{
    if(debug)
        cout << endl << "Solve puzzle1:" << endl;

    Intcode computer(data, 0, false);

    computer.setPipeOutputMode(true);

    long long output(0);

    // We don't know the screen size apriori, so we encode the x,y screen coords into a integer value,
    // and we dump the screentile value into a map.
    int x, y, index;
    map<int, long long> screen;

    int counterTile2 = 0;
    xmin = INT_MAX;
    xmax = INT_MIN;
    ymin = INT_MAX;
    ymax = INT_MIN;

    while(!computer.isHalted())
    {
        x = computer.run();
        y = computer.run();

        index = 100*y + x;
        screen[index] = computer.run();

        if(screen[index] == TILE_BLOCK)
            counterTile2++;

        xmin = std::min(xmin, x);
        ymin = std::min(ymin, y);
        xmax = std::max(xmax, x);
        ymax = std::max(ymax, y);
    }

    if(debug)
    {
        cout << "Solution: nbr of tile 2 type: " << counterTile2 << endl;
    }

    if(debug)
    {
        // We dump the screen to stdout
        cout << "xmin: " << xmin << endl;
        cout << "ymin: " << ymin << endl;

        // Initialise pixel value for ASCII rendering
        pixValue[TILE_EMPTY]  = ' ';
        pixValue[TILE_WALL]   = '#';
        pixValue[TILE_BLOCK]  = '*';
        pixValue[TILE_PADDLE] = '=';
        pixValue[TILE_BALL]   = 'O';

        int widthScreen = xmax - xmin + 1;
        map<long, string> screenStr;

        char buffer[widthScreen];

        for(int i=ymin; i <= ymax; i++)
            screenStr[i] = string(widthScreen+1, ' ');

        for(auto pix : screen)
        {
            index = pix.first;
            x = index % 100;
            y = index / 100;
            strcpy(buffer, screenStr[y].c_str());
            buffer[x] = pixValue[pix.second];

            screenStr[y] = string(buffer);
        }

        for(auto str : screenStr)
        {
            cout << setw(2) << str.first << " : " << str.second << endl;
        }

    }
    return counterTile2;
}

void refreshScreen(map<long, vector<int>>& screen, int x, int y, int newTile, WINDOW* win,bool debug)
{
    if(x == -1)
    {
        screen[x][0] = newTile;
        wmove(win, 0, 0);
        waddstr(win, (string("Score: ") + to_string(newTile)).c_str());
    }
    else
    {
        screen[y][x] = newTile;
        mvwaddch(win, y+2, x+1, pixValue[newTile]);
    }

    wrefresh(win);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    return;
}

// Update paddle position based on ball position.
// The paddle is tracking the ball x position
long long updatePaddlePos(map<long, vector<int>>* screen)
{
    // Find current ball position
    map<long, vector<int>>& s = *screen;

    int ball_offset_x = 0;
    int paddle_offset_x = 0;

    for(auto line : s)
    {
        for(int i = 0; i < line.second.size(); i++)
        {
            if(line.second[i] == TILE_BALL)
            {
                ball_offset_x = i;
            }
            else if(line.second[i] == TILE_PADDLE)
            {
                paddle_offset_x = i;
            }
        }
    }
    return myutils::sgn(ball_offset_x - paddle_offset_x);
}

// Solve puzzle #2
template <typename T>
long long solve_puzzle2(T data, int& xmin, int& xmax, int& ymin, int& ymax, bool debug = false)
{
    if(debug)
    {
        cout << endl << "Solve puzzle2:" << endl;
        cout << "xmin: " << xmin << " : xmax :" << xmax << endl;
        cout << "ymin: " << ymin << " : ymax :" << ymax << endl;
    }

    Intcode<std::vector<long long>, map<long, vector<int>> > computer(data, 0, false);

    computer.setPipeOutputMode(true);

    // Initialize screen to EMPTY_TILE tiles
    map<long, vector<int>> screen;

    for(int y = ymin; y <= ymax; y++)
    {
        vector<int> blankLine(xmax - xmin + 1, TILE_EMPTY);
        screen[y] = blankLine;
    }

    // Input quarter
    computer.setMemory(0, 2);

    // Set input callback function for automatic paddle position updating
    computer.setInputCallbackParam(&screen);
    computer.setInputCallback(updatePaddlePos);

    // Initialise pixel value for ASCII rendering
    pixValue[TILE_EMPTY]  = ' ';
    pixValue[TILE_WALL]   = '#';
    pixValue[TILE_BLOCK]  = '*';
    pixValue[TILE_PADDLE] = '=';
    pixValue[TILE_BALL]   = 'O';

    // Init a curses window for ASCII rendering
    initscr();
    start_color();
    WINDOW *win = newwin(ymax - ymin + 3, xmax - xmin + 3, 2, 2);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    curs_set(0);

    // Let's paddle
    while(!computer.isHalted())
    {
        int x       = computer.run();
        int y       = computer.run();
        int newTile = computer.run();

        refreshScreen(screen, x, y, newTile, win, debug);
    }

    // Terminate curses
    endwin();

    // Return the final score
    return screen[-1][0];
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
    std::vector<long long> data = myutils::read_file_csv<long long, std::vector<long long> >(filename);

    int xmin, xmax, ymin, ymax;

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(data, xmin, xmax, ymin, ymax, true) == 228 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, xmin, xmax, ymin, ymax, false) << std::endl;

    // --------- Puzzle #2 ---------
    assert(solve_puzzle2(data, xmin, xmax, ymin, ymax, true) == 10776 && "Error verifying puzzle #2");

    // Solve puzzle #2
    int sol2 = solve_puzzle2(data, xmin, xmax, ymin, ymax);
    std::cout << "Answer for puzzle #2: "<< sol2 << endl;

}
