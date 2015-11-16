/**
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 *
 * Main
 */

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

static void printCharMap(std::vector<std::vector<int16_t>> map) {
    for (auto x : map) {
        for (auto y : x) {
            printf("%c", convertToChar(y));
        }
        printf("\n");
    }
}

static TerrainGeneration::util::color lerp(TerrainGeneration::util::color c1, TerrainGeneration::util::color c2, float value) {
    std::vector<uint8_t> v(3);

    for (int i = 0; i < 3; i++) {
        if (c1.v[i] > c2.v[i]) {
            v[i] = c2.v[i] + (uint8_t)(float(c1.v[i] - c2.v[i]) * value);
        } else {
            v[i] = c2.v[i] + (uint8_t)(float(c1.v[i] - c2.v[i]) * value);
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

static void printBitmap(uint16_t n, std::vector<std::vector<int16_t>> map) {
    std::ofstream bitmap;
    bitmap.open("result.bmp", std::ofstream::binary);
    if (!bitmap.is_open()) {
        std::cout << "Error opening file result.bmp" << std::endl;
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
    int n = 1025;
    auto map = TerrainGeneration::diamondSquare(n);
    printBitmap(n, map);
    return 0;
}