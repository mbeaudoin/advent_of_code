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
int
read_file(
    T_container& buffer,
    const std::string filename,
    const bool keepSpaces=false,
    const bool debug=false
)
    {
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

        return 0;
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

// Specialized template
template <>
int
myutils::read_file(
    std::vector<std::string>& buffer,
    const std::string filename,
    const bool keepSpaces,
    const bool debug
)
{
    std::ifstream file(filename);

    if(keepSpaces)
    {
        while(file.good())
        {
            std::string line;
            getline(file, line);
            if(line.size() >= 0)
                buffer.push_back(line);
        }
    }
    else
    {
        std::copy(
            std::istream_iterator<std::string>(file),
            std::istream_iterator<std::string>(),
            std::back_inserter(buffer)
        );
    }

    if(debug)
    {
        std::cout << "Buffer len: " << buffer.size() << std::endl;

        for (auto& val : buffer)
            std::cout << val << std::endl;
    }

    return 0;
}

#if 0
    // This is a test for string litterals
    constexpr auto test_data =
        R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)";

    stringstream ss(test_data);

    while(!ss.eof())
    {
        string tt;
        getline(ss, tt, '\n');

        cout << "|" << tt << "|" << endl;
    }
#endif

#endif  // MYUTILS_H
