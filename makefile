#
# name: Kaylie Pham
# RedID: 828129478
#
# name: Aditya Bhagat
# RedID: 828612974

# CXX Make variable for compiler
CXX=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb
CXXFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = log_helpers.o page_table.o page_replacement.o vaddr_tracereader.o pagingwithwsclock.o


# Program name
PROGRAM = pagingwithwsclock

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies

$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $^ -lpthread

log.o : log_helpers.h log_helpers.cpp
	$(CXX) $(CXXFLAGS) log_helpers.cpp

RoboMount.o : page_table.h page_table.cpp
	$(CXX) $(CXXFLAGS) page_table.cpp

Station2.o : wsclock.h wsclock.cpp
	$(CXX) $(CXXFLAGS) page_replacement.cpp

SharedData.o : vaddr_tracereader.h vaddr_tracereader.cpp
	$(CXX) $(CXXFLAGS) vaddr_tracereader.cpp

carassemble.o : pagingwithwsclock.cpp
	$(CXX) $(CXXFLAGS) pagingwithwsclock.cpp

clean :
	rm -f *.o $(PROGRAM)

