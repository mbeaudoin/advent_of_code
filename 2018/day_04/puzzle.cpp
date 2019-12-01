#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <sstream>
#include "include/myutils.h"

using namespace std;

typedef enum {
    AWAKE  = 0,
    ASLEEP = 1,
    BEGIN_SHIFT = 2,
    UNDEFINED = -1
} State;

struct Log {
    int year;
    int month;
    int day;
    int hour;
    int minute;

    State state;

    int guardID;

    Log ()
        : year(-1),
          month(-1),
          day(-1),
          hour(-1),
          minute(-1),
          state(UNDEFINED),
          guardID(-1)
        {
        }

    friend std::istream & operator >>(std::istream & in, Log & l)
    {
        char c;
        std::string cur_state;

        in  >> c
            >> l.year
            >> c
            >> l.month
            >> c
            >> l.day
            >> l.hour
            >> c
            >> l.minute
            >> c ;


        getline(in, cur_state);

        if(cur_state.find("Guard") != std::string::npos)
        {
            std::string scrap;
            std::istringstream ssGuard(cur_state);
            ssGuard >> scrap;
            ssGuard >> c;
            ssGuard >> l.guardID;

            l.state = BEGIN_SHIFT;
        }
        else if(cur_state.find("wakes up") != std::string::npos)
        {
            l.state = AWAKE;
        }
        else
        {
            l.state = ASLEEP;
        }

        return in;
    }


    friend std::ostream& operator<<(std::ostream& os, const Log& l)
    {
        return os << "   Year: " << l.year
            << " : Month: " << l.month
            << " : Day " << l.day
            << " : Hour: " << l.hour
            << " : Minute: " << l.minute
            << " : State: " <<  (l.state == AWAKE? "AWAKE" : (l.state == ASLEEP ? "ASLEEP" : "BEGIN_SHIFT"))
            << " : GuardID: " << l.guardID;
    }

};

int main(int argc, char *argv[])
{
    // Need a command-line parameter for the input filename
    if (argc != 2) {
        cerr << "Error: missing input" << endl;
        return EXIT_FAILURE;
    }

    string filename(argv[1]);

    // This file needs to exist
    if (! myutils::file_exists(filename))
    {
        cerr << "Error: nonexistent file: " << filename << endl;
        return EXIT_FAILURE;
    }

    // Reading the data
    auto listLogs = myutils::read_file<Log, std::vector<Log> >(filename, true);

    // Puzzle #1
    // Classe la liste par valeur de temps
    // sort using a custom lambda function object
    std::sort(listLogs.begin(), listLogs.end(), [] (Log a, Log b)
    {
        if (a.year   != b.year)   return a.year   < b.year;
        if (a.month  != b.month)  return a.month  < b.month;
        if (a.day    != b.day)    return a.day    < b.day;
        if (a.hour   != b.hour)   return a.hour   < b.hour;
        if (a.minute != b.minute) return a.minute < b.minute;
        return false;
    }
    );

    int curGuardID = -1;

    // Fill missing guardID info
    for (auto& l : listLogs)
    {
        if(l.guardID != -1)
        {
            curGuardID = l.guardID;
        }
        else
        {
            l.guardID = curGuardID;
        }
    }


#if 1
    std::cout << "Sorted list" << std::endl;
    for (auto l : listLogs)
    {
        std::cout << l << " ";
    }
    std::cout << std::endl;
#endif

    // Puzzle #1
    // Memorize current shift in a vector of [sleep:awake:sleep:awake:...:sleep:awake]

    std::map<int, std::vector<int> > sleepTimeByGuard;
    for (int i=0; i< listLogs.size(); i++)
    {
        int curGuard = listLogs[i].guardID;

        // New guard ?
        if(sleepTimeByGuard.find(curGuard) == sleepTimeByGuard.end())
        {
            std::vector<int> sTime(60, 0); // 60 entries initialized to 0
            sleepTimeByGuard[curGuard] = sTime;
        }

        if(listLogs[i].state == ASLEEP)
        {
            int startSleep = listLogs[i].minute;
            int stopSleep  = listLogs[i+1].minute;

            for(int j = startSleep; j< stopSleep; j++)
            {
                sleepTimeByGuard[curGuard][j] += 1;
            }

            i++;
        }
    }

    int bestGuardSleeper = -1;
    int maxSleepTime = -1;
    int preferedSleepTime = -1;

    for (auto sTime : sleepTimeByGuard)
    {
#if 1
        std::cout << "Guard: " << sTime.first << " : ";
        for (auto i = sTime.second.begin(); i != sTime.second.end(); ++i)
            std::cout << *i << ' ';
#endif

        int accumulateSleepTime = std::accumulate(sTime.second.begin(), sTime.second.end(), 0);

        // std::cout << accumulateSleepTime;
        // std::cout << std::endl;

        if(accumulateSleepTime > maxSleepTime)
        {
            bestGuardSleeper = sTime.first;
            maxSleepTime = accumulateSleepTime;
            preferedSleepTime = std::distance(sTime.second.begin(), std::max_element(sTime.second.begin(), sTime.second.end()));
        }
    }
    std::cout << "Best sleeper: " << bestGuardSleeper << " : sleepTime : " << maxSleepTime << " : preferedSleepTime:: " << preferedSleepTime << std::endl;

    int answer1 = bestGuardSleeper * preferedSleepTime;
    std::cout << "Answer to puzzle #1 : " << answer1 << std::endl;


    auto answer = 42;

    // Puzzle #2
    std::cout << "Answer puzzle #2: "<< answer << std::endl;
}
