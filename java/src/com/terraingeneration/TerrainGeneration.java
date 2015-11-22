package com.terraingeneration;

import com.terraingeneration.utilities.Color;
import com.terraingeneration.utilities.Utilities;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.*;

/**
 * The terrain generation main class
 * Created by Saul de Nova Caballero on 11/21/15.
 */
public class TerrainGeneration {
    private static Color plain = new Color((byte)0, (byte)64, (byte)0);
    private static Color forest = new Color((byte)133, (byte)182, (byte)116);
    private static Color sea = new Color((byte)0, (byte)0, (byte)55);
    private static Color coast = new Color((byte)0, (byte)53, (byte)106);
    private static Color mount = new Color((byte)167, (byte)157, (byte)147);
    private static Color mountain = new Color((byte)216, (byte)223, (byte)226);

    /**
     * Convert the value into the character
     * @param value the character value
     * @return the character
     */
    private static char convertToChar(short value) {
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

    /**
     * Print the map as a char map to the file map.txt
     * @param map the map to print
     */
    private static void printCharMap(String fileName, short[][] map) {
        try {
            PrintWriter writer = new PrintWriter(fileName, "UTF-8");
            for (short[] row : map) {
                for (short elem : row) {
                    writer.write(convertToChar(elem));
                }
                writer.write('\n');
            }
            writer.close();
        } catch (FileNotFoundException | UnsupportedEncodingException e) {
            System.out.println(e.getLocalizedMessage());
        }
    }

    /**
     * Linear interpolation
     * @param c1 color 1
     * @param c2 color 2
     * @param value interpolation value
     * @return the color
     */
    private static Color lerp(Color c1, Color c2, float value) {
        byte[] v = new byte[3];

        for (int i = 0; i < 3; i++) {
            if (c1.getV()[i] > c2.getV()[i]) {
                v[i] = (byte) (c2.getV()[i] + (byte) ((float)(c1.getV()[i] - c2.getV()[i]) * value));
            } else {
                v[i] = (byte) (c1.getV()[i] + (byte) ((float)(c1.getV()[i] - c2.getV()[i]) * value));
            }
        }

        return new Color(v);
    }

    private static Color convertToColor(short value) {
        value = (short)Utilities.clamp(value, -512, 512);
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
    }

    /**
     * Print the map into a bitmap named map.bmp
     * @param map the map structure
     */
    private static void printBitMap(String fileName, int n, short[][] map) {
        try {
            BufferedImage image = new BufferedImage(n, n, BufferedImage.TYPE_INT_RGB);
            for (int x = 0; x < n; x++) {
                for (int y = 0; y < n; y++) {
                    Color color = convertToColor(map[x][y]);
                    int rgb = (color.getR() & 0xff) << 16 | (color.getG() & 0xff) << 8 | (color.getB() & 0xff);
                    image.setRGB(x, y, rgb);
                }
            }

            ImageIO.write(image, "bmp", new File(fileName));
        } catch (IOException e) {
            System.out.println(e.getLocalizedMessage());
        }
    }

    /**
     * Main function
     * @param args the function arguments
     */
    public static void main(String[] args) {
        long startTime, elapsedSequentialTime, elapsedParallelTime;
        short[][] map;
        int n = 4097;

        try {
            startTime = System.nanoTime();
            map = DiamondSquare.generate(n, false);
            elapsedSequentialTime = System.nanoTime() - startTime;

            printCharMap("map.txt", map);
            printBitMap("map.bmp", n, map);

            System.out.println("Elapsed time: " + elapsedSequentialTime);

            startTime = System.nanoTime();
            map = DiamondSquare.generate(n, true);
            elapsedParallelTime = System.nanoTime() - startTime;

            printCharMap("map_parallel.txt", map);
            printBitMap("map_parallel.bmp", n, map);

            System.out.println("Elapsed parallel time: " + elapsedParallelTime);
            System.out.println("Speedup: " + ((double)elapsedSequentialTime / (double)elapsedParallelTime));
        } catch(IllegalArgumentException e) {
            System.out.println(e.getLocalizedMessage());
        }
    }
}
