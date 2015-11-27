/**
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 *
 * Diamond Square header for the assembly file
 */

namespace TerrainGeneration {
    extern "C" {
        extern void calculateValues(
        	float *results,
        	int32_t size,
        	float *values,
        	float *randoms,
        	float *factors);
    }
}
