/*******************************************************************************
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 ******************************************************************************/

package com.terraingeneration;

import java.util.Random;
import java.util.stream.IntStream;

/**
 * Diamond square implementation
 * Created by Saul de Nova Caballero on 11/22/15.
 */
public class DiamondSquare {

    /**
     * The range of the terrain to obtain
     */
    private static final double ValueRange = 512.0;

    /**
     * The decrement factor of the random influence on the calculations
     */
    private static final double DecrementFactor = 0.8;

    /**
     * A thread local random instance
     */
    private static final ThreadLocal<Random> ThreadLocalRandom = new ThreadLocal<Random>(){
        @Override
        protected Random initialValue() {
            return new Random();
        }
    };

    /**
     * Obtain the next random using a gauss distribution (normal distribution)
     * centered on 0 and using the value range
     * @return the next gaussian value
     */
    private static double randomTerrainGenerator() {
        double result = ThreadLocalRandom.get().nextGaussian();
        result *= ValueRange;
        return result;
    }

    /**
     * Generate a map of size n
     * @param n the size of the map
     * @param parallel if the process is supposed to be parallel
     * @return the generated map
     * @throws IllegalArgumentException
     */
    public static short[][] generate(int n, boolean parallel) throws IllegalArgumentException {
        if (((n - 1) & (n - 2)) != 0) {
            throw new IllegalArgumentException("N must be of the form 2^n + 1");
        }

        short[][] map = new short[n][n];

        map[0][0] = 10;
        map[0][n - 1] = 10;
        map[n - 1][0] = 10;
        map[n - 1][n - 1] = 10;

        if (parallel) {
            double globalFactor = 1.0;

            for (int globalOffset = n - 1; globalOffset > 1; globalOffset /= 2) {
                final int offset = globalOffset;
                final double factor = globalFactor;

                // Diamond generation
                IntStream.range(0, (n - 2) / offset).parallel().forEach(y -> {
                    y *= offset;
                    generateDiamond(n, map, factor, offset, y);
                });

                // Square generation
                IntStream.range(0, (n - 1) / (offset / 2)).parallel().forEach(y -> {
                    y *= (offset / 2);

                    generateSquare(n, map, factor, offset, y);
                });

                globalFactor *= DecrementFactor;
            }
        } else {
            double factor = 1.0;

            for (int offset = n - 1; offset > 1; offset /= 2) {

                // Diamond generation
                for (int y = 0; y < n - 1; y += offset) {
                    generateDiamond(n, map, factor, offset, y);
                }

                // Square generation
                for (int y = 0; y < n; y += offset / 2) {
                    generateSquare(n, map, factor, offset, y);
                }

                factor *= DecrementFactor;
            }
        }

        return map;
    }

    /**
     * Generate the diamond step
     * @param n the size of the map
     * @param map the map
     * @param factor the current decrement factor
     * @param offset the offset of the diamonds
     * @param y the current y
     */
    private static void generateDiamond(
            int n,
            short[][] map,
            double factor,
            int offset,
            int y) {
        for (int x = 0; x < n - 1; x += offset) {
            map[x + (offset / 2)][y + (offset / 2)] = (short) (
                    (map[x][y] + map[x][y + offset] +
                     map[x + offset][y] + map[x + offset][y + offset]) / 4 +
                    randomTerrainGenerator() * factor);
        }
    }

    /**
     * Generate the square step
     * @param n the size of the map
     * @param map the map
     * @param factor the current decrement factor
     * @param offset the offset of the diamonds
     * @param y the current y
     */
    private static void generateSquare(
            int n,
            short[][] map,
            double factor,
            int offset,
            int y) {
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

            map[x][y] = (short) (
                    (result / sides) +
                    randomTerrainGenerator() * factor);
        }
    }
}
