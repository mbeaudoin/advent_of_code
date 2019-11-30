#ifndef MYUTILS_H
#define MYUTILS_H

#include <fstream>
#include <streambuf>
#include <vector>

namespace myutils
{
    template <typename T = std::vector<char>>
    auto read_file(const char* filename)
    {
        auto ifs = std::ifstream(filename);
        return T(std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>{});
    }
}

#endif  // MYUTILS_H
