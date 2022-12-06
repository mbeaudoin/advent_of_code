//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <vector>
#include <deque>
#include <queue>
#include <numeric>
#include <algorithm>
#include <sstream>      // std::stringstream

#include "myutils.h"

using namespace std;

// Stack moves
typedef struct StackMoves
{
    int n;
    int from;
    int to;

    // Default constructor
    StackMoves() : n(-1), from(-1), to(-1) {}

    // Copy constructor
    StackMoves(const StackMoves& m) : n(m.n), from(m.from), to(m.to) {}
} StackMoves;

std::ostream &operator<<(std::ostream &os, const StackMoves &m)
{
    os << "n: " << m.n << " from:" << m.from << " to:" << m.to;
    return os;
}


// Stacks
typedef struct Stacks
{
    std::vector<std::deque<char>> stacks;

    // Constructor
    Stacks(int n) : stacks(n) {}

    // Add item to stack 'i'
    void addToStack(int index, char item)
    {
        stacks[index].push_back(item);
    }

    // Move from stack to stack, not preserving order for multiple crates (Mover9000)
    void doMoveCrateMover9000(const StackMoves& m)
    {
        if(m.from > stacks.size() || m.to > stacks.size())
        {
            cout << "Error: invalid stacks index for move " << this << endl;
        }
        else
        {
            std::list<char> transF;

            // Remove from stack, store in transF
            for(int i=0; i<m.n; i++)
            {
                transF.push_front(stacks[m.from-1].front());
                stacks[m.from-1].pop_front();
            }

            // Transfer to stack
            for(int i=0; i<m.n; i++)
            {
                stacks[m.to-1].push_front(transF.back());
                transF.pop_back();
            }
        }

        return;
    }

    // Move from stack to stack, preserving order for multiple crates (Mover9001)
    void doMoveCrateMover9001(const StackMoves& m)
    {
        if(m.from > stacks.size() || m.to > stacks.size())
        {
            cout << "Error: invalid stacks index for move " << this << endl;
        }
        else
        {
            std::list<char> transF;

            // Remove from stack, store in transF
            for(int i=0; i<m.n; i++)
            {
                transF.push_back(stacks[m.from-1].front());
                stacks[m.from-1].pop_front();
            }

            // Transfer to stack
            for(int i=0; i<m.n; i++)
            {
                stacks[m.to-1].push_front(transF.back());
                transF.pop_back();
            }
        }

        return;
    }

    string getTopCrates()
    {
        string retValue;
        for(auto s : stacks)
        {
            if(! s.empty())
            {
                retValue += s.front();
            }
            else
            {
                retValue += ' ';
            }
        }

        return retValue;
    }

} Stacks;

std::ostream &operator<<(std::ostream &os, const Stacks &s)
{
    for(auto s : s.stacks)
    {
        for(auto c : s)
        {
            os << c << ":";
        }
        os << endl;
    }
    return os;
}

// Solve puzzle #1 or #2 based on crane model
template <typename T>
std::string solve_puzzle(T& data, int craneModel, bool debug = false)
{
    int nbrStacks = 0;

    int indexLineNumbers = 0;

    // First, the stacks block
    for(auto val: data)
    {
        if(debug)
            cout << "Val: " << val << endl;

        if(std::strncmp(val.c_str(), " 1 ", 3) == 0)
        {
            break;
        }
        indexLineNumbers++;
    }

    // Compute the number of data columns
    int iMax = data.begin()->size();

    for(int i = 1; i< iMax; i+=4)
    {
        nbrStacks++;
    }

    if(debug)
        cout << "nbrStacks: " << nbrStacks << endl;

    // Allocate initial stacks
    Stacks stacks(nbrStacks);

    for(int i=0; i<indexLineNumbers; i++)
    {
        int offset = 1;
        int stackIndex = 0;

        for(int offset=1; offset < data[i].size(); offset +=4)
        {
            char c = data[i][offset];
            if(c != ' ')
            {
                stacks.addToStack(stackIndex, c);
            }
            stackIndex++;
        }
    }

    if(debug)
    {
        cout << "Initial stacks: " << endl;
        cout << stacks << endl;
    }

    // Stack moves
    std::queue<StackMoves> moves;

    // Grab moves
    for(auto val: data)
    {
        if(std::strncmp(val.c_str(), "move", 4) == 0)
        {
            std::stringstream ss(val);
            StackMoves m;
            string dummyStr;

            // Extract move information
            // Example : move 1 from 2 to 1
            ss >> dummyStr >> m.n >> dummyStr >> m.from >> dummyStr >> m.to;
            moves.push(m);

            if(debug)
                cout << "Memorize stack moves: " << m << endl;

        }
    }

    // Handle moves
    while (!moves.empty())
    {
        StackMoves m = moves.front();

        if(debug)
            std::cout << "Execute stack move: " << m << endl;

        craneModel == 9000 ? stacks.doMoveCrateMover9000(m) : stacks.doMoveCrateMover9001(m);

        if(debug)
        {
            cout << "Current stacks state: " << endl;
            cout << stacks << endl;
        }

        moves.pop();
    }
    if(debug)
        std::cout << '\n';


    // Top crates:
    string topCrates = stacks.getTopCrates();

    if(debug)
        cout << "Top crates : " << topCrates << endl;

    return topCrates;
}

// Solve puzzle #1
template <typename T>
std::string solve_puzzle1(T& data)
{
    // Use crane model 9000
    return solve_puzzle<T>(data, 9000);
}


// Solve puzzle #2
template <typename T>
std::string solve_puzzle2(T& data)
{
    // Use crane model 9001
    return solve_puzzle<T>(data, 9001);
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

    bool keepSpaces = true;
    bool keepEmptyLines = false;
    myutils::read_file(data, filename, keepSpaces, keepEmptyLines);

    const vector<string> example = {
        "    [D]    ",
        "[N] [C]    ",
        "[Z] [M] [P]",
        " 1   2   3 ",
        "",
        "move 1 from 2 to 1",
        "move 3 from 1 to 3",
        "move 2 from 2 to 1",
        "move 1 from 1 to 2"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(example) == "CMZ" && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(example) == "MCD" && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
