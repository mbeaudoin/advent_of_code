# Compilation variables
CXX=g++
CPPFLAGS= -I../include
CXXFLAGS= -std=c++17 -O2 $(CPPFLAGS)

# wget command and args to retrieve the day's input data
# using the input session cookie from ${HOME}/.aoc_session_cookie
WGET=wget
WGET_ARGS=--load-cookies ${HOME}/.aoc_session_cookie

# Extract the day number from the current directory name
CUR_DIR:=$(notdir $(CURDIR))
DAY_NUM:=$(subst day_0, , $(CUR_DIR))
DAY_NUM:=$(strip $(subst day_, , $(DAY_NUM)))

# AOC event year
AOC_EVENT_YEAR=2019

# AOC day's input datafile name
INPUT_DATAFILE=input.txt

# Input datafile URL
AOC_INPUT_URL=https://adventofcode.com/$(AOC_EVENT_YEAR)/day/$(DAY_NUM)/input

SRCS = puzzle.cpp
OBJS = $(SRCS:.cpp=.o)

all: getdata puzzle run

puzzle: $(OBJS)
	@echo "Compiling puzzle"
	$(CXX) -o puzzle $(CXXFLAGS) $(OBJS)

run: getdata puzzle
	@echo "Solving puzzle"
	./puzzle $(INPUT_DATAFILE)

getdata:
	@echo "Getting data for day # $(DAY_NUM)"
	test -f $(INPUT_DATAFILE) || $(WGET) $(WGET_ARGS) -O $(INPUT_DATAFILE) $(AOC_INPUT_URL)

clean:
	@echo "Cleaning up"
	rm -f puzzle $(OBJS) *~ $(INPUT_DATAFILE) Makefile.bak

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

depend:
	makedepend -- $(CPPFLAGS) -- $(SRCS)
# DO NOT DELETE

puzzle.o: ../include/myutils.h ../include/Intcode.h
