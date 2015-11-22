package com.terraingeneration;

import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;

/**
 * Diamond square implementation
 * Created by Saul de Nova Caballero on 11/22/15.
 */
public class DiamondSquare {

    private static final double ValueRange = 512.0;

    private static final double DecrementFactor = 0.8;

    private static final ThreadLocal<Random> ThreadLocalRandom = new ThreadLocal<Random>(){
        @Override
        protected Random initialValue() {
            return new Random();
        }
    };

    private static double randomTerrainGenerator() {
        double result = ThreadLocalRandom.get().nextGaussian();
        result *= ValueRange;
        return result;
    }

    public static short[][] generate(int n) throws IllegalArgumentException {
        if (((n - 1) & (n - 2)) != 0) {
            throw new IllegalArgumentException("N must be of the form 2^n + 1");
        }

        short[][] map = new short[n][n];

        double factor = 1.0;

        map[0][0] = 10;
        map[0][n - 1] = 10;
        map[n - 1][0] = 10;
        map[n - 1][n - 1] = 10;

        for (int offset = n - 1; offset > 1; offset /= 2) {

            // Diamond generation
            for (int y = 0; y < n - 1; y += offset) {
                for (int x = 0; x < n - 1; x += offset) {
                    map[x + (offset / 2)][y + (offset / 2)] = (short)((map[x][y] + map[x][y + offset] + map[x + offset][y] + map[x + offset][y + offset]) / 4 +
                            randomTerrainGenerator() * factor);
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

                    map[x][y] = (short)((result / sides) + randomTerrainGenerator() * factor);
                }
            }

            factor *= DecrementFactor;
        }

        return map;
    }
}
