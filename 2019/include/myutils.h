#ifndef MYUTILS_H
#define MYUTILS_H

#include <fstream>
#include <streambuf>
#include <iterator>
#include <vector>
#include <algorithm>

namespace myutils
{

template <typename T_value = char,
          typename T_container = std::vector<T_value>>
auto read_file(const char* filename, const bool debug=false)
    {
        T_container buffer;

        std::ifstream file(filename);

        std::copy(
            std::istream_iterator<T_value>(file),
            std::istream_iterator<T_value>(),
            std::back_inserter(buffer)
        );

        if(debug)
        {
            std::cout << "Buffer len: " << buffer.size() << std::endl;

            for (auto& val : buffer)
                std::cout << val << std::endl;
        }

        return buffer;
    }
}

#endif  // MYUTILS_H
