/**
 * Apache License Version 2.0
 *
 * Saul de Nova Caballero
 *
 * The utilities header file
 */

#include <cstdint>
#include <vector>

namespace TerrainGeneration {
    namespace util {
        template <class T>
        struct vec2 {
            T x;
            T y;

            vec2(T x, T y) : x(x), y(y) { }
            vec2() { }
        };

        template <class T>
        struct vec3 {
            T x;
            T y;
            T z;

            vec3(T x, T y, T z) : x(x), y(y), z(z) { }
            vec3() { }
        };    

        struct color {
            const uint8_t r;
            const uint8_t g;
            const uint8_t b;
            const uint8_t v[3];

            color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), v{r, g, b} { }
            color(const std::vector<uint8_t> & v) : r(v[0]), g(v[1]), b(v[2]), v{v[0], v[1], v[2]} { }
        };

        template <class T>
        T clamp(const T& n, const T& lower, const T& upper) {
            return std::max(lower, std::min(n, upper));
        }
    }
}
