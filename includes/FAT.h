//
// Created by Vladimir on 06.06.2022.
//

#ifndef MYFS_FAT_H
#define MYFS_FAT_H

#include "myfs-structs.h"

class FAT {
private:
    BlockDevice *blockDevice;
    // map entry to next corresponding data block
    // 'FAT_EOF' to mark end of file
    int32_t fat_array[DATA_BLOCKS];
    int modifiedBlocks[DATA_BLOCKS]; // tracking modified blocks currently in memory
    int modifiedBlocksCounter = 0;

public:
    FAT(BlockDevice *device);
    ~FAT();

    void insert_modified_block(int index);
    void clear_modified_blocks();

    int set_next_block(int currentBlock, int nextBlock);
    int get_next_block(int currentBlock);

    void free_block(int index);

    void save_on_disk();

    void init_fat();
    void first_init_fat();
};

#endif //MYFS_FAT_H
