#include "libseq.h"

#include <iostream>
#include <iomanip>

namespace distcalc::sequence
{
    distance_t distance(block_t* buffer, size_t blocks_count)
    {
        distance_t dist(0);
        interblock_t xlast, ylast;
        for (size_t bid(0); bid < blocks_count; bid++) {
            auto block = reinterpret_cast<Block*>(buffer + bid);
            if (bid) {
                dist += euclideanDistance(Point{block->x1, block->y1}, Point{xlast, ylast});
            }

            dist += euclideanDistance(Point{block->x1, block->y1}, Point{block->x2, block->y2});
            xlast = block->x2, ylast = block->y2;
        }
        return round(dist);
    }
}
