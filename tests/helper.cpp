#include "helper.h"
#include <random>


namespace distcalc::unittests
{
    std::pair<std::vector<block_t>, double> makeSample(size_t blocks_count, unsigned seed)
    {
        double dist(0);
        std::vector<block_t> vals(blocks_count);
        interblock_t xlast, ylast;
        std::mt19937 rng(seed);
        std::uniform_int_distribution<block_t> uid(-127, 128);
        for (size_t bid(0); bid < blocks_count; bid++) {
            auto [x, y] = std::make_pair(uid(rng), uid(rng));
            if (bid) {
                dist += euclideanDistance(Point{xlast, ylast},
                                          Point{static_cast<interblock_t>(x),
                                                static_cast<interblock_t>(y)});
            }
            xlast = uid(rng), ylast = uid(rng);
            dist += euclideanDistance(Point{static_cast<interblock_t>(x),
                                            static_cast<interblock_t>(y)},
                                      Point{xlast, ylast});
            block_t block = ((static_cast<block_t>(x) & 0xFF) << 24) +
                            ((static_cast<block_t>(y) & 0xFF) << 16) +
                            ((static_cast<block_t>(xlast) & 0xFF) << 8) +
                            (static_cast<block_t>(ylast) & 0xFF);
            vals[bid] = block;
        }
        return { vals, round(dist) };
    }
}
