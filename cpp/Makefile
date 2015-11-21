CXX = g++
CXXFLAGS = -Wall -g
CXX11 = -std=c++0x
PARALLELFLAGS = -D PARALLEL -fopenmp

all: terrain_generation_parallel terrain_generation

terrain_generation_parallel: parallel/diamond_square.o parallel/main.o parallel/utilities.o
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -o terrain_generation_parallel parallel/diamond_square.o parallel/main.o parallel/utilities.o

parallel/diamond_square.o: diamond_square.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -c diamond_square.cxx
	mkdir -p parallel
	mv diamond_square.o parallel/diamond_square.o

parallel/main.o: main.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -c main.cxx 
	mkdir -p parallel
	mv main.o parallel/main.o

parallel/utilities.o: utilities/utilities.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -c utilities/utilities.cxx
	mkdir -p parallel
	mv utilities.o parallel/utilities.o

terrain_generation: diamond_square.o main.o utilities.o
	$(CXX) $(CXXFLAGS) $(CXX11) -o terrain_generation diamond_square.o main.o utilities.o

diamond_square.o: diamond_square.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) -c diamond_square.cxx

main.o: main.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) -c main.cxx

utilities.o: utilities/utilities.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) -c utilities/utilities.cxx
