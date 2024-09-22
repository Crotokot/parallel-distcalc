#include <iomanip>
#include "libpar.h"

int main(int argc, char* argv[])
{
    if (argc == 4) {
        size_t pid = static_cast<size_t>(std::atoi(argv[1]));
        size_t offset = static_cast<size_t>(std::atoi(argv[2]));
        size_t blocks_count = static_cast<size_t>(std::atoi(argv[3]));
        auto res = distcalc::parallel::process(offset, blocks_count);
        bipc::shared_memory_object shmem(bipc::open_only, RESULTID, bipc::read_write);
        bipc::mapped_region region(shmem, bipc::read_write);
        auto results = static_cast<distcalc::distance_t*>(region.get_address());
        results[pid] = res;
    }
    return 0;
}
