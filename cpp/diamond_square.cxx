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
#include <random>
#include <queue>

#include "diamond_square.h"
#include "utilities/utilities.h"

namespace TerrainGeneration {

    /**
     * Diamond square implementation.
     * n and m are the sizes of the diamond square and must be equal and must 
     * be $2^x - 1$
     */
    std::vector<std::vector<int16_t>> diamondSquare(uint16_t n) {
        if (((n - 1) & (n - 2)) != 0) {
            throw 0;
        }

        double range = 256.0;

        std::queue<util::vec2<int16_t>> points;

        std::vector<std::vector<int16_t>> map(n, std::vector<int16_t>(n, 0));

        // Generate random numbers
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> distribution(0, range / 2);
        double factor = 1.0f;

        auto terrain_gen = std::bind(distribution, generator);

        map[0][0] = 10;
        map[0][n - 1] = 10;
        map[n - 1][0] = 10;
        map[n - 1][n - 1] = 10;

        for (int offset = n - 1; offset > 1; offset /= 2) {

            // Diamond generation
            for (int y = 0; y < n - 1; y += offset) {
                for (int x = 0; x < n - 1; x += offset) {
                    map[x + (offset / 2)][y + (offset / 2)] = (map[x][y] + map[x][y + offset] + map[x + offset][y] + map[x + offset][y + offset]) / 4 +
                                                              terrain_gen() * factor;
                }
            }

            // Square generation
            for (int y = 0; y < n; y += offset / 2) {
                for (int x = (y + (offset / 2)) % offset; x < n; x += offset) {
                    double sides = 4.0;
                    double result = 0.0;
                    if (x == 0) {
                        sides = 3.0;
                    } else {
                        result += map[x - (offset / 2)][y];
                    }

                    if (x == n - 1) {
                        sides = 3.0;
                    } else {
                        result += map[x + (offset / 2)][y];
                    }

                    if (y == 0) {
                        sides = 3.0;
                    } else {
                        result += map[x][y - (offset / 2)];
                    }

                    if (y == n - 1) {
                        sides = 3.0;
                    } else {
                        result += map[x][y + (offset / 2)];
                    }

                    map[x][y] = (result / sides) + terrain_gen() * factor;
                }
            }

            factor *= 0.8f;
        }

        return map;
    }
}
