/*******************************************************************************
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 ******************************************************************************/

package com.terraingeneration.utilities;

/**
 * Generic utility functions
 * Created by Saul de Nova Caballero on 11/22/15.
 */
public class Utilities {

    /**
     * Limit the value n between lower and upper
     * @param n value
     * @param lower lower limit
     * @param upper upper limit
     * @return the value n clamped
     */
    public static int clamp(int n, int lower, int upper) {
        return Math.max(lower, Math.min(n, upper));
    }
}
