/*******************************************************************************
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 ******************************************************************************/

package com.terraingeneration.utilities;

import java.util.Arrays;

/**
 * Color class
 * Created by Saul de Nova Caballero on 11/21/15.
 */
public class Color {
    /**
     * Red
     */
    private byte r;

    /**
     * Green
     */
    private byte g;

    /**
     * Blue
     */
    private byte b;

    /**
     * Byte vector
     */
    private byte[] v;

    /**
     * Default constructor
     * @param r red
     * @param g green
     * @param b blue
     */
    public Color(byte r, byte g, byte b) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.v = new byte[3];
        this.v[0] = r;
        this.v[1] = g;
        this.v[2] = b;
    }

    /**
     * Default constructor
     * @param v vector array
     */
    public Color(byte[] v) {
        this.r = v[0];
        this.g = v[1];
        this.b = v[2];
        this.v = Arrays.copyOf(v, 3);
    }

    /**
     * Get the red component
     * @return the red component
     */
    public byte getR() {
        return this.r;
    }

    /**
     * Get the green component
     * @return the green component
     */
    public byte getG() {
        return this.g;
    }

    /**
     * Get the blue component
     * @return the blue component
     */
    public byte getB() {
        return this.b;
    }

    /**
     * Get the vector array
     * @return the color vector array
     */
    public byte[] getV() {
        return Arrays.copyOf(v, 3);
    }
}
