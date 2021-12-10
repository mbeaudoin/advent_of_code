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

typedef enum
{
    ON,
    OFF,
    TOGGLE
} Command;

class Instruction
{
public:
    Command command;   // Command
    myutils::point c1; // Corner points
    myutils::point c2;

    // Default constructor
    Instruction() {};

    // Copy constructor
    Instruction(const Instruction &b)
    {
        command = b.command;
        c1      = b.c1;
        c2      = b.c2;
    };

};

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    vector<Instruction> instructions(0);

    for(auto d : data)
    {
        Instruction instr;

        cout << d << endl;

        size_t last_index = d.find_first_of("0123456789");
        string result = d.substr(last_index - 1);
        cout << result << endl;

        myutils::point c1, c2;  // Corner points
        string c_scrap;
        char sep;
        std::stringstream ss(d.substr(last_index - 1));
        ss >> instr.c1.x >> sep >> instr.c1.y
            >> c_scrap
            >> instr.c2.x >> sep >> instr.c2.y;

        if(d.find("turn on") != string::npos)
        {
            instr.command = ON;
        }
        else if(d.find("turn off") != string::npos)
        {
            instr.command = OFF;
        }
        else if(d.find("toggle") != string::npos)
        {
            instr.command = TOGGLE;
        }
        else
        {
            cout << " Error: bad command : "  << d << endl;
        }

        instructions.push_back(instr);
    }

    bool grid[1000][1000];
    return 42;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    return 42;
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
    //const auto example1 = "1";
    //assert(solve_puzzle1<vector<string>>({example1}) == 42 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const auto example2 = "2";
    assert(solve_puzzle2<vector<string>>({example2}) == 42 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
