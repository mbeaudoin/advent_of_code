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
    T data_;

public:
    Intcode (T data)
    : data_(data)
    {
    }

    int evaluate()
    {
        int currentIndex = 0;
        int opCode = data_[currentIndex++];

        while(opCode != 99)
        {
            int indexParam1 = data_[currentIndex++];
            int indexParam2 = data_[currentIndex++];
            int indexParam3 = data_[currentIndex++];

            if(opCode == 1)  // 1: Addition
            {
                data_[indexParam3] = data_[indexParam1] + data_[indexParam2];
            }
            else             // 2: multiplication
            {
                data_[indexParam3] = data_[indexParam1] * data_[indexParam2];
            }

            opCode = data_[currentIndex++];
        }

            return data_[0];
    }
};

#endif  // MYUTILS_H
