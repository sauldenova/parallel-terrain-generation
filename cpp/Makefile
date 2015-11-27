CXX = g++
CXXFLAGS = -Wall -g
CXX11 = -std=c++0x
PARALLELFLAGS = -fopenmp
NASM = nasm
NASM_ARCH = -f elf64

all: terrain_generation

terrain_generation: diamond_square.o main.o utilities.o diamond_square_asm.o
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -o terrain_generation diamond_square.o main.o utilities.o diamond_square_asm.o

diamond_square.o: diamond_square.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -c diamond_square.cxx

main.o: main.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -c main.cxx

utilities.o: utilities/utilities.cxx
	$(CXX) $(CXXFLAGS) $(CXX11) $(PARALLELFLAGS) -c utilities/utilities.cxx

diamond_square_asm.o: diamond_square_asm.asm
	$(NASM) $(NASM_ARCH) diamond_square_asm.asm
