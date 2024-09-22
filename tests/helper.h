#pragma once

#include <chrono>
#include "basics.h"
#include <iostream>
#include <iomanip>

namespace distcalc::unittests
{
    std::pair<std::vector<block_t>, double>
    makeSample(size_t blocks_count, unsigned seed = 42);

    template<class DistanceFunc, class... Args>
    double distanceDuration(DistanceFunc& distance, Args... args)
    {
        size_t fulltime(0);
        unsigned retry(10);
        for (unsigned i(0); i < retry; i++) {
            auto start = std::chrono::steady_clock::now();
            distance(args...);
            auto finish = std::chrono::steady_clock::now();
            fulltime += std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
        }
        return static_cast<double>(fulltime) / 1e6;
    }
}
