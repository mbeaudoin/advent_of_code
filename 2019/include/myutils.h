#ifndef MYUTILS_H
#define MYUTILS_H

#include <fstream>
#include <streambuf>
#include <iterator>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>

namespace myutils
{

template <typename T_value = char,
          typename T_container = std::vector<T_value>>
    auto read_file(const std::string filename, const bool debug=false)
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


template <typename T_value = char,
    typename T_container = std::vector<T_value>>
    auto read_file_csv(const std::string filename, const bool debug=false, char sepValue=',')
    {
        T_container buffer;

        std::ifstream file(filename);

        std::string number_as_string;

        while (std::getline(file, number_as_string, sepValue))
        {
            buffer.push_back(std::stoi(number_as_string));
        }

        if(debug)
        {
            std::cout << "Buffer len: " << buffer.size() << std::endl;

            for (auto& val : buffer)
                std::cout << val << std::endl;
        }

        return buffer;
    }

inline bool file_exists (const std::string& name)
    {
        return ( access( name.c_str(), F_OK ) != -1 );
    }

}
#endif  // MYUTILS_H
