//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <vector>
#include <stack>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <iterator>

using namespace std;

#ifndef INTCODE_H
#define INTCODE_H

typedef std::vector<int> paramMode;
static const int MAX_NBR_PARAM = 3;

std::ostream& operator<<(std::ostream& os, const paramMode& p)
{
    for(int i=0; i<MAX_NBR_PARAM; i++)
    {
        os << p[MAX_NBR_PARAM - i - 1];
    }
    return os;
}

template <class T = std::vector<long long>>
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
        RELBASE  = 9,
        HALT     = 99,
        FEEDTHEPIPE = 88
    };

    enum paramIndex
    {
        PARAM1 = 0,
        PARAM2 = 1,
        PARAM3 = 2
    };

    enum paramModeValue
    {
        POSITION  = 0,
        IMMEDIATE = 1,
        RELATIVE  = 2
    };

    enum runningState
    {
        RUNNING,
        HALTED
    };

    //- Internal variables

    const int largeMemorySize_ = 20000;

    // Computer memory
    T memory_;

    // Instruction pointer
    long long ip_;

    // Current instruction parameter mode
    paramMode paramMode_;

    // Input
    stack<long long> input_;  // Allow multiple inputs

    // Last output
    long long output_;

    // Pipe mode       We interrupt execution after opcode OUTPUT.
    bool pipeMode_;

    // Running state
    runningState runState_;

    // Relative base for relative mode
    long long relBase_;

    // Debug mode
    bool debug_;

    long long extractParamMode(long long& opCode)
    {
        int paramModeVal = opCode / 100;

        // Convert paramModeVal into a leading zero string 001
        std::stringstream ss;
        ss << std::setw(MAX_NBR_PARAM) << std::setfill('0') << paramModeVal;
        std::string s = ss.str();

        // Move info into paramMode vector
        paramMode_[0] = s[2] - '0';
        paramMode_[1] = s[1] - '0';
        paramMode_[2] = s[0] - '0';

        opCode %= 100;

        return opCode;
    }

    // Reset parameter mode to POSITION
    void resetParamMode()
    {
        paramMode_.clear();
        for(int i=0; i<MAX_NBR_PARAM; i++)
            paramMode_.push_back(POSITION);
    }

    // Extract parameter value base on current paramMode
    long long extractParamIndex(long long initialParamValue, paramIndex index)
    {
        if(debug_)
            cout << "Inside extractParamIndex: ip_: " << ip_ << endl;

        long long paramIndex = initialParamValue;

        switch(paramMode_[index])
        {
            case POSITION:
            {
                if(debug_)
                    cout << "Param: " << index << ": mode : POSITION: " << " : memory_[" << initialParamValue << "]" << ": " << memory_[initialParamValue] << endl;
                // Do nothing, the parameter already specify the proper index
            }
            break;
            case IMMEDIATE:
            {
                if(debug_)
                    cout << "Param: " << index << ": IMMEDIATE: " << initialParamValue << endl;

                paramIndex = ip_-1;
            }
            break;
            case RELATIVE:
            {
                if(debug_)
                {
                    cout << "Param: " << index << ": RELATIVE: " << initialParamValue <<  ": tot. offset: " << initialParamValue + relBase_
                        << " : memory_[" << initialParamValue + relBase_ << "]" << ": "<< memory_[initialParamValue + relBase_] << endl;
                }
                paramIndex = initialParamValue + relBase_;
            }
            break;
            default:
                cout << "!!!!!!!!!!!!! -> Error: Bad parameter mode: " << paramMode_[index] << endl;
        }

        return paramIndex;
    }

public:

    // Constructor

    // Single input program
    Intcode (T& initState, long long input, bool debug = false)
        : memory_(initState),
          ip_(0),
          paramMode_(MAX_NBR_PARAM),
          output_(0),
          pipeMode_(false),
          runState_(RUNNING),
          relBase_(0),
          debug_(debug)
    {
        input_.push(input);

        // Resize for large memory
        memory_.resize(memory_.size() + largeMemorySize_, 0);
    }

    // Multiple inputs program
    Intcode (T& initState, T input, bool debug = false)
        : memory_(initState),
          ip_(0),
          paramMode_(MAX_NBR_PARAM ),
          output_(0),
          pipeMode_(false),
          runState_(RUNNING),
          relBase_(0),
          debug_(debug)
    {
        for(auto i : input)
            input_.push(i);

        // Resize for large memory
        memory_.resize(memory_.size() + largeMemorySize_, 0);
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
    long long run()
    {
        long long opCode = 0;  // opCode

        while(opCode != HALT && opCode != FEEDTHEPIPE)
        {
            if(debug_)
                cout << "ip_: " << ip_ << " ---- ";

            // Reset parameter mode to POSITION
            resetParamMode();

            // Next opcode
            opCode = memory_[ip_++];

            if(opCode > 100)
            {
                // We have some parameter in immediate mode
                opCode = extractParamMode(opCode);
            }

            if(debug_)
                cout << "opcode: " << opCode << " - paramMode: " << paramMode_ << endl;

            switch (opCode)
            {
                case ADD:   // 1
                {
                    // Extract parameter index
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);
                    long long indexParam2 = extractParamIndex(memory_[ip_++], PARAM2);
                    long long indexParam3 = extractParamIndex(memory_[ip_++], PARAM3);

                    memory_[indexParam3] = memory_[indexParam1] + memory_[indexParam2];
                }
                break;

                case MULT:  // 2
                {
                    // Extract parameter index
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);
                    long long indexParam2 = extractParamIndex(memory_[ip_++], PARAM2);
                    long long indexParam3 = extractParamIndex(memory_[ip_++], PARAM3);

                    memory_[indexParam3] = memory_[indexParam1] * memory_[indexParam2];
                }
                break;

                case INPUT:  // 3
                {
                    if(input_.size() > 0)
                    {
                        long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);

                        memory_[indexParam1] = input_.top();

                        if(debug_)
                            cout << "INPUT:: value: " << input_.top() << " at index: " << indexParam1 << endl;

                        input_.pop();
                    }
                    else
                    {
                        // We request some input, but the input stack is empty.
                        std::cout << "Error:  empty input stack" << std::endl;
                    }
                }
                break;

                case OUTPUT:  // 4
                {
                    // Check param mode
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);

                    if(debug_)
                        cout << "OUTPUT: indexParam1: " << indexParam1 << endl;

                    output_ = memory_[indexParam1];

                    if(pipeMode_)
                    {
                        // Stop current execution
                        opCode = FEEDTHEPIPE;

                        // Memorize instruction pointer
                        ip_;
                    }

                    if(debug_)
                    {
                        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>  Output: "
                        << output_
                        << std::endl;
                    }
                }
                break;

                case JMPTRUE:  // 5
                {
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);
                    long long indexParam2 = extractParamIndex(memory_[ip_++], PARAM2);

                    if(memory_[indexParam1] > 0)
                    {
                        ip_ = memory_[indexParam2];
                    }
                    //else
                    //    ip_ +=2;
                }
                break;

                case JMPFALSE:  // 6
                {
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);
                    long long indexParam2 = extractParamIndex(memory_[ip_++], PARAM2);

                    if(memory_[indexParam1] == 0)
                    {
                        ip_ = memory_[indexParam2];
                    }
                    //else
                    //    ip_ +=2;
                }
                break;

                case LESS:   // 7
                {
                    // Extract parameter index
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);
                    long long indexParam2 = extractParamIndex(memory_[ip_++], PARAM2);
                    long long indexParam3 = extractParamIndex(memory_[ip_++], PARAM3);

                    memory_[indexParam3] = (memory_[indexParam1] < memory_[indexParam2]) ? 1 : 0;
                }
                break;

                case EQUALS:   // 8
                {
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);
                    long long indexParam2 = extractParamIndex(memory_[ip_++], PARAM2);
                    long long indexParam3 = extractParamIndex(memory_[ip_++], PARAM3);

                    memory_[indexParam3] = (memory_[indexParam1] == memory_[indexParam2]) ? 1 : 0;
                }
                break;

                case RELBASE:   // 9
                {

                    // Check param mode
                    long long indexParam1 = extractParamIndex(memory_[ip_++], PARAM1);

                    if(debug_)
                    {
                        cout << "indexParam1: " << indexParam1 << endl;
                        cout << "relBase_ old value: " << relBase_ << endl;
                    }

                    relBase_ += memory_[indexParam1];

                    if(debug_)
                        cout << "relBase_ new value: " << relBase_ << endl;
                }
                break;

                case HALT:   // 99
                    runState_ = HALTED;
                    break;

                default:
                    std::cout << "Error: unsupported opcode: " << opCode
                        << ": Aborting." << std::endl;
            }
        }

        return output_;
    }

    // Return memory content at index
    long long pokeMemory(long long index)
    {
        return memory_[index];
    }
};


#endif  // MYUTILS_H
