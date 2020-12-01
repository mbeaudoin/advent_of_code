//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#ifndef MYUTILS_H
#define MYUTILS_H

#include <fstream>
#include <streambuf>
#include <iterator>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

namespace myutils
{

template <typename T_value = char,
          typename T_container = std::vector<T_value>>
auto read_file(const std::string filename, bool keepSpaces=false, const bool debug=false)
    {
        T_container buffer;

        std::ifstream file(filename);

        if(keepSpaces)
        {
#if 0
            while(file.good())
            {
                std::string line;
                getline(file, line);
                if(line.size() > 0)
                    buffer.push_back(line);
            }
#endif
        }
        else
        {
            std::copy(
                std::istream_iterator<T_value>(file),
                std::istream_iterator<T_value>(),
                std::back_inserter(buffer)
            );
        }

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
            buffer.push_back(std::stoll(number_as_string));
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


    // 2D point class
    class point
    {
    public:
        int x;
        int y;

        point() : x(0), y(0) {}
        point(int valx, int valy) : x(valx), y(valy) {}
        point(const point& p) : x(p.x), y(p.y) {}
        point (point&& p): x(std::move(p.x)), y(std::move(p.y)) {}

        point& operator=(point other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        point operator+(point other)
        {
            other.x += x;
            other.y += y;

            return other;
        }

        // Distance squared from origin
        int dist2() const
        {
            return x*x + y*y;
        }

        // compare for order.
        bool operator <(const point& pt) const
        {
            return pt.dist2() > dist2();
        }

    };

    bool operator==(const point &p1, const point& p2)
    {
        return p1.x == p2.x && p1.y == p2.y;
    }

    bool operator!=(const point &p1, const point& p2)
    {
        return p1.x != p2.x || p1.y != p2.y;
    }

    double atan2InDeg(const point &p)
    {
        double deg = atan2(p.y, p.x) * 180.0 / M_PI;

        return deg;
    }

    std::ostream& operator<<(std::ostream& os, const point& p)
    {
        return os << p.x
            << " "
            << p.y;
    }

    template <typename T> int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }
}
#endif  // MYUTILS_H
