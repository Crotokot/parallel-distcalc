#include "basics.h"

namespace distcalc
{
    distance_t euclideanDistance(const Point& p1, const Point& p2)
    {
        auto dist = std::sqrt(std::pow(static_cast<distance_t>(p1.x) - p2.x, 2) +
                              std::pow(static_cast<distance_t>(p1.y) - p2.y, 2));
        return dist;
    }

    block_t* memalloc(size_t blocks_count)
    {
        block_t *buffer = new block_t[blocks_count];
        return buffer;
    }

    void memfree(block_t* buffer)
    {
        delete [] buffer;
    }

    void memfill(block_t* buffer, size_t blocks_count, block_t val)
    {
        std::fill(buffer, buffer + blocks_count, val);
    }

    void memfill(block_t* buffer, size_t blocks_count, const std::vector<block_t>& vals)
    {
        for (size_t bid(0); bid < blocks_count; bid++)
            buffer[bid] = vals[bid];
    }

    distance_t round(distance_t dist, unsigned precision)
    {
        auto norm = std::pow(10, precision);
        return std::round(dist * norm) / norm;
    }
}
