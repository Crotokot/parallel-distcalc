#include <chrono>
#include <iostream>
#include <iomanip>
#include <gtest/gtest.h>
#include "helper.h"


#ifdef USE_LIBSEQ
#include "libseq.h"
#endif

#ifdef USE_LIBPAR
#include "libpar.h"
#endif

namespace distcalc::unittests
{
    #ifdef USE_LIBSEQ
    TEST(LIBSEQ, NullDist) {
        size_t blocks_count(100);
        auto buffer = memalloc(blocks_count);
        memfill(buffer, blocks_count, 0);
        auto dist = sequence::distance(buffer, blocks_count);
        memfree(buffer);
        EXPECT_DOUBLE_EQ(dist, 0);
    }

    TEST(LIBSEQ, DistEquality) {
        size_t blocks_count(25 * 1024);
        auto [vals, res_dist] = makeSample(blocks_count);
        auto buffer = memalloc(blocks_count);
        memfill(buffer, blocks_count, vals);
        auto dist = sequence::distance(buffer, blocks_count);
        memfree(buffer);
        EXPECT_DOUBLE_EQ(dist, res_dist);
    }
    #endif

    #ifdef USE_LIBPAR
    TEST(LIBPAR, NullDist) {
        size_t blocks_count(100);
        auto buffer = memalloc(blocks_count);
        memfill(buffer, blocks_count, 0);
        auto dist = parallel::distance(buffer, blocks_count);
        memfree(buffer);
        EXPECT_DOUBLE_EQ(dist, 0);
    }

    TEST(LIBPAR, DistEquality) {
        size_t blocks_count(25 * 1023);
        auto [vals, res_dist] = makeSample(blocks_count);
        auto buffer = memalloc(blocks_count);
        memfill(buffer, blocks_count, vals);
        auto dist = parallel::distance(buffer, blocks_count);
        memfree(buffer);
        EXPECT_DOUBLE_EQ(dist, res_dist);
    }

    #ifdef USE_LIBSEQ
    TEST(DISTCALC, CompareRealizations) {
        unsigned max_process(2 * std::thread::hardware_concurrency());
        std::cout << "Time comparison (seconds):" << std::endl;
        std::cout << "Number of cores: " << std::thread::hardware_concurrency() << std::endl;
        std::stringstream table_header;
        table_header << "Buffer size (MB)\tSequential";
        for (unsigned nb_process(2); nb_process <= max_process; nb_process++)
            table_header << "\tParallel (proc=" << std::to_string(nb_process) << ")";
        std::cout << table_header.str() << std::endl;
        for (size_t mbbytes_count(2); mbbytes_count <= 4; mbbytes_count++) {
            size_t blocks_count((mbbytes_count * 1024 * 1024) / 4);
            auto [vals, res_dist] = makeSample(blocks_count);
            auto buffer = memalloc(blocks_count);
            memfill(buffer, blocks_count, vals);
            auto seqseconds = distanceDuration(sequence::distance, buffer, blocks_count);
            std::cout << std::setw(16) << mbbytes_count << "\t" << std::setw(10) << seqseconds;

            for (unsigned nb_process(2); nb_process <= max_process; nb_process++) {
                auto parseconds = distanceDuration(parallel::distance, buffer, blocks_count, nb_process);
                std::cout << "\t" << std::setw(16) << parseconds;
            }
            std::cout << std::endl;

            memfree(buffer);
        }
    }
    #endif
    #endif

}
