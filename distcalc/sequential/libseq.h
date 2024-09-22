#pragma once

#include <tuple>
#include <random>
#include <iostream>
#include <algorithm>
#include "basics.h"

namespace distcalc::sequence
{
    distance_t distance(block_t* buffer, size_t blocks_count);
}
