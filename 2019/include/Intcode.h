//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <vector>

#ifndef INTCODE_H
#define INTCODE_H

template <class T = std::vector<int>>
class Intcode
{
private:

    // Supported opcodes
    enum opcodes
    {
        ADD  = 1,
        MULT = 2,
        HALT = 99
    };

    // Computer memory
    T memory_;

public:
    Intcode (T& initState)
    : memory_(initState)
    {
    }

    // Evaluate the current program state
    int evaluate()
    {
        int ip = 0;  // Instruction pointer

        int opCode = 0;  // opCode

        while(opCode != HALT)
        {
            // Next opcode
            opCode = memory_[ip++];

            switch (opCode)
            {
                case ADD:
                {
                    int instructParam1 = memory_[ip++];
                    int instructParam2 = memory_[ip++];
                    int instructParam3 = memory_[ip++];
                    memory_[instructParam3] = memory_[instructParam1] + memory_[instructParam2];
                }
                break;

                case MULT:
                {
                    int instructParam1 = memory_[ip++];
                    int instructParam2 = memory_[ip++];
                    int instructParam3 = memory_[ip++];
                    memory_[instructParam3] = memory_[instructParam1] * memory_[instructParam2];
                }
                break;

                case HALT:
                    break;

                default:
                    std::cout << "Error: unsupported opcode: " << opCode
                        << ": Aborting." << std::endl;
            }
        }

        return memory_[0];
    }
};

#endif  // MYUTILS_H
