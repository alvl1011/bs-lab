//
// Created by Vladimir on 06.06.2022.
//

#ifndef MYFS_DMAP_H
#define MYFS_DMAP_H

#include "blockdevice.h"
#include "myfs-structs.h"
#include "Utils.h"

class DMAP {
private:
    BlockDevice *blockDevice;
    bool blocks[DATA_BLOCKS]; // array of all data blocks, contains true if the block is in use
    int blockInUseCounter = DATA_BLOCKS; // keep tracking of all blocks which are currently in use

public:
    DMAP(BlockDevice *device);
    ~DMAP();

    int get_next_free_block();
    int* get_amount_free_blocks(int amount);

    void set_block_state(int dataBlockNumber, bool isUsed);
    bool get_block_state(int dataBlockNumber);

    void increase_free_block_counter_by(int amount);
    void decrease_free_block_counter_by(int amount);

    bool save_on_disk();

    void init_dmap();
    void first_init_dmap();
};

#endif //MYFS_DMAP_H