#pragma once

#include <tuple>
#include <random>
#include <thread>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/search_path.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "basics.h"

#define MEMID "BufferSharedMemory.bin"
#define RESULTID "BufferResultMemory"

namespace bp = boost::process;
namespace bipc = boost::interprocess;

namespace distcalc::parallel
{
    distance_t distance(block_t* buffer,
                        size_t blocks_count,
                        unsigned nb_process = std::thread::hardware_concurrency());

    distance_t process(size_t offset, size_t blocks_count);
}
