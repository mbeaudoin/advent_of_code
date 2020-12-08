//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>
#include "myutils.h"

using namespace std;

typedef enum
{
    NOP,
    ACC,
    JMP
} OpCode;

typedef std::tuple<OpCode, int, bool> instruction;
typedef vector<instruction> program;

// Computer error codes
# define E_OK             0
# define E_INFINITE_LOOP -1

class computer
{
private:
    int progCounter;
    int accumulator;

public:
    computer() : progCounter(0), accumulator(0) {}

    void reset()
    {
        progCounter = 0;
        accumulator = 0;
    }

    int getAccumulator()
    {
        return accumulator;
    }

    int runProgram(program& prog)
    {
        bool stop = false;
        int eCode = E_OK;

        while(!stop)
        {
            // Infinite loop detection
            if(std::get<2>(prog[progCounter]))
            {
                stop = true;
                eCode = E_INFINITE_LOOP;
            }
            else
            {
                // Leave marker for infinite loop
                std::get<2>(prog[progCounter]) = true;

                switch(std::get<0>(prog[progCounter]))
                {
                    case NOP:
                        progCounter++;
                        break;
                    case ACC:
                        accumulator += get<1>(prog[progCounter]);
                        progCounter++;
                        break;
                    case JMP:
                        progCounter += get<1>(prog[progCounter]);
                        break;
                }
            }

            if(progCounter >= prog.size())
                stop = true;
        }

        return eCode;
    }
};

template <typename T>
void convertData(T data, program& prog)
{
    map<string, OpCode> _strToOpCode = {
        {"nop", NOP},
        {"acc", ACC},
        {"jmp", JMP}
    };

    for(auto d : data)
    {
        stringstream ss(d);

        string opcode;
        int value;

        ss >> opcode >> value;

        //cout << "Opcode: " << opcode << " : Value: " << value <<  endl;

        prog.push_back(std::make_tuple(_strToOpCode[opcode], value, false));
    }
    return;
}

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    program prog;
    computer comp;
    int accumulValue;

    convertData(data, prog);
    comp.runProgram(prog);

    return comp.getAccumulator();
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    program prog;   // Reference program
    program modProg;

    computer comp;

    convertData(data, prog);

    int iOpCodeModif = 0;

    do
    {
        modProg = prog;
        comp.reset();

        while(iOpCodeModif++ < modProg.size())
        {
            if(std::get<0>(modProg[iOpCodeModif]) == NOP)
            {
                get<0>(modProg[iOpCodeModif]) = JMP;
                break;
            }
            else if(std::get<0>(modProg[iOpCodeModif]) == JMP)
            {
                get<0>(modProg[iOpCodeModif]) = NOP;
                break;
            }
        }
    }
    while(comp.runProgram(modProg) != E_OK);

    return comp.getAccumulator();
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
    const vector<string> example1 = {
        "nop +0",
        "acc +1",
        "jmp +4",
        "acc +3",
        "jmp -3",
        "acc -99",
        "acc +1",
        "jmp -4",
        "acc +6"
    };

    // Verify puzzle1 example
    assert(solve_puzzle1(example1) == 5 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 example
    assert(solve_puzzle2(example1) == 8 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
