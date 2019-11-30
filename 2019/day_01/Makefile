# Compilation variables
CC=g++
CPPFLAGS= -I..
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
AOC_EVENT_YEAR=2018

# AOC day's input datafile name
INPUT_DATAFILE=input.txt

# Input datafile URL
AOC_INPUT_URL=https://adventofcode.com/$(AOC_EVENT_YEAR)/day/$(DAY_NUM)/input

all: getdata puzzle1 run1 puzzle2 run2

puzzle1:
	@echo "Compiling puzzle #1"
	$(CC) -o puzzle1 $(CXXFLAGS) puzzle1.cpp

puzzle2:
	@echo "Compiling puzzle #2"
	$(CC) -o puzzle2 $(CXXFLAGS) puzzle2.cpp

run1: getdata puzzle1
	@echo "Solving puzzle #1"
	./puzzle1 $(INPUT_DATAFILE)

run2: getdata puzzle2
	@echo "Solving puzzle #2"
	./puzzle2 $(INPUT_DATAFILE)

getdata:
	@echo "Getting data for day # $(DAY_NUM)"
	$(WGET) $(WGET_ARGS) -O $(INPUT_DATAFILE) $(AOC_INPUT_URL)

clean:
	@echo "Cleaning up"
	rm -f puzzle1 puzzle2 *~ $(INPUT_DATAFILE)
