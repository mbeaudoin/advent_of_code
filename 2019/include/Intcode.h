//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <vector>
#include <bitset>
#include <stack>

using namespace std;

#ifndef INTCODE_H
#define INTCODE_H

template <class T = std::vector<int>>
class Intcode
{
private:

    // Supported opcodes
    enum opcodes
    {
        ADD      = 1,
        MULT     = 2,
        INPUT    = 3,
        OUTPUT   = 4,
        JMPTRUE  = 5,
        JMPFALSE = 6,
        LESS     = 7,
        EQUALS   = 8,
        HALT     = 99,
        FEEDTHEPIPE = 88
    };

    enum paramModeValue
    {
        POSITION  = 0,
        IMMEDIATE = 1
    };

    enum runningState
    {
        RUNNING,
        HALTED
    };

    static const int MAX_NBR_PARAM = 3;
    typedef std::bitset<MAX_NBR_PARAM> paramMode;

    //- Internal variables

    // Computer memory
    T memory_;

    // Instruction pointer
    int ip_;

    // Current instruction parameter mode
    paramMode paramMode_;

    // Input
    stack<int> input_;  // Allow multiple inputs

    // Last output
    int output_;

    // Pipe mode       We interrupt execution after opcode OUTPUT.
    bool pipeMode_;

    // Running state
    runningState runState_;

    // Debug mode
    bool debug_;

    int extractParamMode(int& opCode)
    {
        int paramModeVal = opCode / 100;
        //std::cout << "opCode: " << opCode << " - paramMode: " << paramModeVal << std::endl;

        // Move info into bitfield
        paramMode_ = paramMode(to_string(paramModeVal));

        //cout << "paramMode_: " << paramMode_ << endl;

        opCode %= 100;

        //cout << "opCode: " << opCode << endl;

        return opCode;
    }

    // Reset parameter mode to POSITION
    void resetParamMode()
    {
        paramMode_ = POSITION;
    }

public:

    // Constructor

    // Single input program
    Intcode (T& initState,int input, bool debug = false)
        : memory_(initState),
          ip_(0),
          paramMode_(0),
          output_(0),
          pipeMode_(false),
          runState_(RUNNING),
          debug_(debug)
    {
        input_.push(input);
    }

    // Multiple inputs program
    Intcode (T& initState, T input, bool debug = false)
        : memory_(initState),
          ip_(0),
          paramMode_(0),
          output_(0),
          pipeMode_(false),
          runState_(RUNNING),
          debug_(debug)
    {
        for(auto i : input)
            input_.push(i);
    }

    void setPipeOutputMode(bool pMode)
    {
        pipeMode_ = pMode;

        if(debug_)
            cout << "pipeMode: " << pipeMode_ << endl;
    }

    void setInput(T input)
    {
        for(auto i : input)
            input_.push(i);
    }

    bool isHalted()
    {
        return runState_ == HALTED;
    }

    // Run the current program state
    int run()
    {
        int ip = ip_;  // Current instruction pointer

        int opCode = 0;  // opCode

        while(opCode != HALT && opCode != FEEDTHEPIPE)
        {
            // Reset parameter mode to POSITION
            resetParamMode();

            // Next opcode
            opCode = memory_[ip++];

            if(opCode > 100)
            {
                // We have some parameter in immediate mode
                opCode = extractParamMode(opCode);
            }

            if(debug_)
                cout << "opcode: " << opCode << " - paramMode: " << paramMode_ << endl;

            switch (opCode)
            {
                case ADD:
                {
                    int instructParam1 = memory_[ip++];
                    int instructParam2 = memory_[ip++];
                    int instructParam3 = memory_[ip++];

                    // Check param mode
                    int value1 = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;
                    int value2 = paramMode_[1] == POSITION ? memory_[instructParam2] : instructParam2;

                    memory_[instructParam3] = value1 + value2;
                }
                break;

                case MULT:
                {
                    int instructParam1 = memory_[ip++];
                    int instructParam2 = memory_[ip++];
                    int instructParam3 = memory_[ip++];

                    // Check param mode
                    int value1 = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;
                    int value2 = paramMode_[1] == POSITION ? memory_[instructParam2] : instructParam2;

                    memory_[instructParam3] = value1 * value2;
                }
                break;

                case INPUT:
                {
                    if(input_.size() > 0)
                    {
                        int instructParam1 = memory_[ip++];
                        memory_[instructParam1] = input_.top();
                        input_.pop();
                    }
                    else
                    {
                        // We request some input, but the input stack is empty.
                        std::cout << "Error:  empty input stack" << std::endl;
                    }
                }
                break;

                case OUTPUT:
                {
                    int instructParam1 = memory_[ip++];

                    // Check param mode
                    output_ = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;

                    if(pipeMode_)
                    {
                        // Stop current execution
                        opCode = FEEDTHEPIPE;

                        // Memorize instruction pointer
                        ip_ = ip;
                    }

                    if(debug_)
                    {
                        std::cout << ">>>>>>>>>>>  Output: "
                        << output_
                        << std::endl;
                    }
                }
                break;

                case JMPTRUE:
                {
                    int instructParam1 = memory_[ip];
                    int instructParam2 = memory_[ip+1];

                    // Check param mode
                    int value1 = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;
                    int value2 = paramMode_[1] == POSITION ? memory_[instructParam2] : instructParam2;

                    if(value1 > 0)
                    {
                        ip = value2;
                    }
                    else
                        ip +=2;
                }
                break;

                case JMPFALSE:
                {
                    int instructParam1 = memory_[ip];
                    int instructParam2 = memory_[ip+1];

                    // Check param mode
                    int value1 = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;
                    int value2 = paramMode_[1] == POSITION ? memory_[instructParam2] : instructParam2;

                    if(value1 == 0)
                    {
                        ip = value2;
                    }
                    else
                        ip +=2;
                }
                break;

                case LESS:
                {
                    int instructParam1 = memory_[ip++];
                    int instructParam2 = memory_[ip++];
                    int instructParam3 = memory_[ip++];

                    // Check param mode
                    int value1 = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;
                    int value2 = paramMode_[1] == POSITION ? memory_[instructParam2] : instructParam2;

                    memory_[instructParam3] = (value1 < value2) ? 1 : 0;
                }
                break;

                case EQUALS:
                {
                    int instructParam1 = memory_[ip++];
                    int instructParam2 = memory_[ip++];
                    int instructParam3 = memory_[ip++];

                    // Check param mode
                    int value1 = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;
                    int value2 = paramMode_[1] == POSITION ? memory_[instructParam2] : instructParam2;

                    memory_[instructParam3] = (value1 == value2) ? 1 : 0;
                }
                break;

                case HALT:
                    runState_ = HALTED;
                    break;

                default:
                    std::cout << "Error: unsupported opcode: " << opCode
                        << ": Aborting." << std::endl;
            }
        }

        return output_;
    }
};

#endif  // MYUTILS_H
