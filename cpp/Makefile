CXX = g++
CXXFLAGS = -Wall -g
CXX11 = -std=c++0x

all: diamond_square.o main.o utilities.o
	$(CXX) $(CXXFLAGS) $(CXX11) -o terrain_generation diamond_square.o main.o utilities.o

diamond_square.o: diamond_square.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) -c diamond_square.cxx

main.o: main.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) -c main.cxx

utilities.o: utilities/utilities.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) -c utilities/utilities.cxx
