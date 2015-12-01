/**
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 *
 * Diamond Square header
 */

#include <vector>

namespace TerrainGeneration {

    enum class ParallelType { NONE, OPENMP, ASSEMBLY };

    std::vector<std::vector<int16_t>> diamondSquare(
        uint16_t n, 
        TerrainGeneration::ParallelType isParallel);
}
