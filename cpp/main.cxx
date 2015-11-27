/**
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 *
 * Main
 */

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
 
#include "diamond_square.h"
#include "utilities/utilities.h"

static TerrainGeneration::util::color plain(0,64,0), forest(116,182,133), sea(55,0,0), coast(106,53,0), mount(147,157,167), mountain(226,223,216);

static char convertToChar(int16_t value) {
    if (value >= 256) {
        return '^';
    } else if (value >= 128) {
        return 'h';
    } else if (value >= 0) {
        return 'p';
    } else if (value >= -128) {
        return ',';
    }
    return '.';
}

static void printCharMap(std::string fileName, std::vector<std::vector<int16_t>> map) {
    FILE * outputFile = fopen(fileName.c_str(), "w");
    for (auto x : map) {
        for (auto y : x) {
            fprintf(outputFile, "%c", convertToChar(y));
        }
        fprintf(outputFile, "\n");
    }
    fclose(outputFile);
}

static TerrainGeneration::util::color lerp(TerrainGeneration::util::color c1, TerrainGeneration::util::color c2, float value) {
    std::vector<uint8_t> v(3);

    for (int i = 0; i < 3; i++) {
        if (c1.v[i] > c2.v[i]) {
            v[i] = c2.v[i] + (uint8_t)(float(c1.v[i] - c2.v[i]) * value);
        } else {
            v[i] = c1.v[i] + (uint8_t)(float(c1.v[i] - c2.v[i]) * value);
        }
    }

    return TerrainGeneration::util::color(v);
}

static TerrainGeneration::util::color convertToColor(int16_t value) {
    value = TerrainGeneration::util::clamp<int16_t>(value, -512, 512);
    if (value >= 256) {
        return lerp(mountain, mount, (value - 256) / 256);
    } else if (value >= 128) {
        return lerp(mount, forest, (value - 128) / 128);
    } else if (value >= 0) {
        return lerp(forest, plain, value / 128);
    } else if (value >= -128) {
        return lerp(plain, coast, (value + 128) / 128);
    }
    return lerp(coast, sea, (value + 512) / 384);
    //return TerrainGeneration::util::color(0, 0, 0);
}

static void printBitmap(std::string fileName, uint16_t n, std::vector<std::vector<int16_t>> map) {
    std::ofstream bitmap;
    bitmap.open(fileName, std::ofstream::binary);
    if (!bitmap.is_open()) {
        std::cout << "Error opening file " << fileName << std::endl;
        exit(0);
    }

    // Bitmap header
    // File type/unused/offset/unused
    bitmap.put(char(66));
    bitmap.put(char(77));
    for (int i = 0; i < 8; i++) {
        bitmap.put(char(0));
    }
    
    bitmap.put(char(54));
    for (int i = 0; i < 3; i++) {
        bitmap.put(char(0));
    }
    
    bitmap.put(char(40));
    for (int i = 0; i < 3; i++) {
        bitmap.put(char(0));
    }
    
    // File width
    bitmap.put(char(n % 256));
    bitmap.put(char((n >> 8) % 256));
    bitmap.put(char((n >> 16) % 256));
    bitmap.put(char((n >> 24) % 256));
    // File height
    bitmap.put(char(n % 256));
    bitmap.put(char((n >> 8) % 256));
    bitmap.put(char((n >> 16) % 256));
    bitmap.put(char((n >> 24) % 256));
    // Color planes/Bit depth
    bitmap.put('\1');
    bitmap.put('\0');
    bitmap.put(char(24));
    // Unused
    for (int i = 0; i < 25; i++) {
        bitmap.put('\0');
    }

    for (int y = n - 1; y >= 0; y--) {
        for (int x = 0; x < n; x++) {
            TerrainGeneration::util::color color = convertToColor(map[x][y]);
            bitmap.put(char(color.r));
            bitmap.put(char(color.g));
            bitmap.put(char(color.b));
        }

        for (int i = 0; i < (n % 4); i++) {
            bitmap.put('\0');
        }
    }

    bitmap.close();
}

int main(int argc, char** argv) {
    std::vector<std::vector<int16_t>> map;
    struct timespec tstart={0,0}, tend={0,0};
    int n = 4097;

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    map = TerrainGeneration::diamondSquare(n, TerrainGeneration::ParallelType::NONE);
    clock_gettime(CLOCK_MONOTONIC, &tend);

    printCharMap("map.txt", map);
    printBitmap("map.bmp", n, map);

    double passedTime = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

    printf("Elapsed sequential time: %.5lf\n", passedTime);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    map = TerrainGeneration::diamondSquare(n, TerrainGeneration::ParallelType::OPENMP);
    clock_gettime(CLOCK_MONOTONIC, &tend);

    printCharMap("map_parallel.txt", map);
    printBitmap("map_parallel.bmp", n, map);

    double passedTimeParallel = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

    printf("Elapsed parallel time: %.5lf\n", passedTimeParallel);

    printf("Speedup: %.5lf\n", passedTime / passedTimeParallel);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    map = TerrainGeneration::diamondSquare(n, TerrainGeneration::ParallelType::ASSEMBLY);
    clock_gettime(CLOCK_MONOTONIC, &tend);

    printCharMap("map_assembly.txt", map);
    printBitmap("map_assembly.bmp", n, map);

    double passedTimeAssembly = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

    printf("Elapsed assembly time: %.5lf\n", passedTimeAssembly);

    printf("Speedup assembly: %.5lf\n", passedTime / passedTimeAssembly);

    return 0;
}