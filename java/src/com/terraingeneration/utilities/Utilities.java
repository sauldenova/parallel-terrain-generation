/*******************************************************************************
 * Copyright(c) 2015 ITESM CEM.
 * <p>
 * All rights reserved
 ******************************************************************************/

package com.terraingeneration.utilities;

/**
 * Generic utility functions
 * Created by Saul de Nova Caballero on 11/22/15.
 */
public class Utilities {
    public static int clamp(int n, int lower, int upper) {
        return Math.max(lower, Math.min(n, upper));
    }
}
