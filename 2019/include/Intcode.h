//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <vector>
#include <bitset>

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
        HALT     = 99
    };

    enum paramModeValue
    {
        POSITION  = 0,
        IMMEDIATE = 1
    };

    static const int MAX_NBR_PARAM = 3;
    typedef std::bitset<MAX_NBR_PARAM> paramMode;

    //- Internal variables

    // Computer memory
    T memory_;

    // Current instruction parameter mode
    paramMode paramMode_;

    // Input
    int input_;

    // Last output
    int output_;

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
    Intcode (T& initState, int input, bool debug = false)
        : memory_(initState),
          paramMode_(0),
          input_(input),
          output_(0),
          debug_(debug)
    {
    }

    // Evaluate the current program state
    int evaluate()
    {
        int ip = 0;  // Instruction pointer

        int opCode = 0;  // opCode

        while(opCode != HALT)
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

            // cout << "opcode: " << opCode << " - paramMode: " << paramMode_ << endl;

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
                    int instructParam1 = memory_[ip++];

                    memory_[instructParam1] = input_;
                }
                break;

                case OUTPUT:
                {
                    int instructParam1 = memory_[ip++];

                    // Check param mode
                    output_ = paramMode_[0] == POSITION ? memory_[instructParam1] : instructParam1;

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
