#include "libpar.h"
#include <iostream>

namespace distcalc::parallel
{
    void sharedMemAlloc(block_t *buffer, size_t blocks_count)
    {
        {
            bipc::file_mapping::remove(MEMID);
            std::filebuf fbuf;
            fbuf.open(MEMID, std::ios_base::in | std::ios_base::out
                                | std::ios_base::trunc | std::ios_base::binary);

            fbuf.pubseekoff(blocks_count * sizeof(distcalc::block_t), std::ios_base::beg);
            fbuf.sputc(0);
        }

        bipc::file_mapping m_file(MEMID, bipc::read_write);
        bipc::mapped_region region(m_file, bipc::read_write);
        auto sharedmem = static_cast<distcalc::block_t*>(region.get_address());
        for (size_t bid(0); bid < blocks_count; bid++)
            sharedmem[bid] = buffer[bid];
        region.flush();
    }

    void sharedMemFree()
    {
        bipc::file_mapping::remove(MEMID);
    }

    distance_t distance(block_t *buffer,
                        size_t blocks_count,
                        unsigned nb_process)
    {
        sharedMemAlloc(buffer, blocks_count);
        bipc::shared_memory_object::remove(RESULTID);
        bipc::shared_memory_object subdists{bipc::create_only, RESULTID, bipc::read_write};
        subdists.truncate(nb_process * sizeof(distcalc::distance_t));
        std::vector<bp::child> children;
        size_t offset(0), process_blocks_count(blocks_count / nb_process);
        size_t remain_blocks_count(blocks_count % nb_process);
        if (remain_blocks_count)
            process_blocks_count += 1;
        for (unsigned pid(0); pid < nb_process; pid++) {
            std::stringstream command;
            boost::filesystem::path process = bp::search_path("process");
            command << process << " " << pid << " " << offset << " " << process_blocks_count;
            children.push_back(bp::child(command.str()));
            offset += process_blocks_count;
            if (remain_blocks_count > 0) {
                remain_blocks_count--;
                if (!remain_blocks_count)
                    process_blocks_count -= 1;
            }
        }
        for (auto& ch: children)
            ch.wait();

        bipc::mapped_region region(subdists, bipc::read_write);
        auto results = static_cast<distance_t*>(region.get_address());
        distance_t dist(0);
        for (unsigned pid(0); pid < nb_process; pid++)
            dist += results[pid];

        bipc::shared_memory_object::remove(RESULTID);
        sharedMemFree();
        return round(dist);
    }

    distance_t process(size_t offset, size_t blocks_count)
    {
        bipc::file_mapping shmem(MEMID, bipc::read_only);
        size_t block_size(blocks_count * sizeof(block_t));
        size_t byte_offset = offset * sizeof(block_t);
        bipc::mapped_region region(shmem, bipc::read_only, byte_offset, block_size);
        auto buffer = static_cast<block_t*>(region.get_address());
        distcalc::distance_t dist(0);
        distcalc::interblock_t xlast, ylast;
        if (byte_offset) {
            auto start = byte_offset - sizeof(block_t);
            auto size = 2 * sizeof(block_t);
            bipc::mapped_region region(shmem, bipc::read_only, start, size);
            auto buffer = static_cast<block_t*>(region.get_address());
            auto block1 = reinterpret_cast<Block*>(buffer);
            auto block2 = reinterpret_cast<Block*>(buffer + 1);
            dist += euclideanDistance(Point{block1->x2, block1->y2}, Point{block2->x1, block2->y1});
        }
        for (size_t bid(0); bid < blocks_count; bid++) {
            auto block = reinterpret_cast<Block*>(buffer + bid);
            if (bid) {
                dist += euclideanDistance(Point{block->x1, block->y1}, Point{xlast, ylast});
            }
            dist += euclideanDistance(Point{block->x1, block->y1}, Point{block->x2, block->y2});
            xlast = block->x2, ylast = block->y2;
        }
        return dist;
    }
}
