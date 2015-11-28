/**
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 *
 * Diamond Square implementation
 */

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <random>
#include <queue>

#include <omp.h>

#include "diamond_square.h"
#include "diamond_square_asm.h"
#include "utilities/utilities.h"

namespace TerrainGeneration {

    /**
     * Diamond square implementation.
     * n and m are the sizes of the diamond square and must be equal and must 
     * be $2^x - 1$
     */
    std::vector<std::vector<int16_t>> diamondSquare(uint16_t n, TerrainGeneration::ParallelType type) {
        if (((n - 1) & (n - 2)) != 0) {
            throw 0;
        }

        float range = 256.0;

        std::vector<std::vector<int16_t>> map(n, std::vector<int16_t>(n, 0));

        // Generate random numbers
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> distribution(0, range / 2);
        float factor = 1.0f;

        int pointer = 0;
        float *values  = (float *)malloc(sizeof(float) * n * n);
        float *factors = (float *)malloc(sizeof(float) * n * n);
        float *results = (float *)malloc(sizeof(float) * n * n);
        float *randoms = (float *)malloc(sizeof(float) * n * n);
        util::vec2<int> *points = new util::vec2<int>[n * n];

        auto terrain_gen = std::bind(distribution, generator);

        map[0][0] = 10;
        map[0][n - 1] = 10;
        map[n - 1][0] = 10;
        map[n - 1][n - 1] = 10;

        if (type != TerrainGeneration::ParallelType::OPENMP) {
            omp_set_num_threads(1);
        }

        for (int offset = n - 1; offset > 1; offset /= 2) {

            // Diamond generation
            #pragma omp parallel for shared(map, terrain_gen, factor, offset)
            for (int y = 0; y < n - 1; y += offset) {
                for (int x = 0; x < n - 1; x += offset) {
                    float value = (map[x][y] + map[x][y + offset] + map[x + offset][y] + map[x + offset][y + offset]) / 4.0;
                    float random = terrain_gen();
                    if (type != TerrainGeneration::ParallelType::ASSEMBLY) {
                        map[x + (offset / 2)][y + (offset / 2)] = value + random * factor;
                    } else {
                        points[pointer] = util::vec2<int>(x + (offset / 2), y + (offset / 2));
                        values[pointer] = value;
                        randoms[pointer] = random;
                        factors[pointer] = factor;
                        pointer++;
                    }
                }
            }

            if (pointer > 0) {
                int p = pointer;
                if ((p % 4) != 0) {
                    p += (4 - (pointer % 4));
                }
                
                calculateValues(results, p, values, randoms, factors);
                for (int i = 0; i < pointer; i++) {
                    map[points[i].x][points[i].y] = results[i];
                }
                pointer = 0;
            }

            // Square generation
            #pragma omp parallel for shared(map, terrain_gen, factor, offset)
            for (int y = 0; y < n; y += offset / 2) {
                for (int x = (y + (offset / 2)) % offset; x < n; x += offset) {
                    double result = 0.0;
                    if (x != 0) {
                        result += map[x - (offset / 2)][y];
                    }

                    if (x != n - 1) {
                        result += map[x + (offset / 2)][y];
                    }

                    if (y != 0) {
                        result += map[x][y - (offset / 2)];
                    }

                    if (y != n - 1) {
                        result += map[x][y + (offset / 2)];
                    }

                    float value = result / 4.0;
                    float random = terrain_gen();
                    if (type != TerrainGeneration::ParallelType::ASSEMBLY) {
                        map[x][y] = value + random * factor;
                    } else {
                        points[pointer] = util::vec2<int>(x, y);
                        values[pointer] = value;
                        randoms[pointer] = random;
                        factors[pointer] = factor;
                        pointer++;
                    }
                }
            }

            if (pointer > 0) {
                int p = pointer;
                if ((p % 4) != 0) {
                    p += (4 - (pointer % 4));
                }

                calculateValues(results, p, values, randoms, factors);
                for (int i = 0; i < pointer; i++) {
                    map[points[i].x][points[i].y] = results[i];
                }
                pointer = 0;
            }

            factor *= 0.8f;
        }
        return map;
    }
}
