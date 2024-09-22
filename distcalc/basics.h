#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

namespace distcalc
{
    typedef int32_t block_t;
    typedef int8_t interblock_t;
    typedef double distance_t;

    struct Block
    {
        interblock_t y2, x2, y1, x1;
    };

    struct Point
    {
        interblock_t x, y;
    };

    distance_t euclideanDistance(const Point& p1, const Point& p2);

    block_t* memalloc(size_t blocks_count);
    void memfree(block_t*);
    void memfill(block_t* buffer, size_t blocks_count, block_t val);
    void memfill(block_t* buffer, size_t blocks_count, const std::vector<block_t>& vals);
    distance_t round(distance_t dist, unsigned precision = 2);
}
