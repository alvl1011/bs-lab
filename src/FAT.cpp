//
// Created by Vladimir on 10.06.2022.
//

#include <FAT.h>

/// @brief FAT constructor
/// init blockDevice for current object
/// @param blockDevice
FAT::FAT(BlockDevice *blockDevice) {
    this->blockDevice = blockDevice;
    for(int i = 0; i < DATA_BLOCKS; i++)
        fat_array[i] = FAT_EOF;
}

/// @brief Destructor of FAT
FAT::~FAT() = default;

/// @brief Insert modified block in FAT
/// @param index
void FAT::insert_modified_block(int index) {
    // verify that index is not present already
    bool isPresent = false;
    for(int i = 0; i < modifiedBlocksCounter; i++) {
        if(modifiedBlocks[i] == index) isPresent = true;
    }

    // track change
    if(!isPresent) {
        modifiedBlocks[modifiedBlocksCounter] = index;
        modifiedBlocksCounter++;
    }
}

/// @brief Clears modified blocks
void FAT::clear_modified_blocks() {
    for(int i = 0; i < modifiedBlocksCounter; i++)
        modifiedBlocks[i] = 0;
    modifiedBlocksCounter = 0;
}

/// @brief Set next block in FAT array
/// @param currentBlock
/// @param nextBlock
/// @return 0 if successful -1 if not
int FAT::set_next_block(int currentBlock, int nextBlock) {

    if(currentBlock == nextBlock)
        return -1;
    this->fat_array[currentBlock] = nextBlock;
    insert_modified_block(currentBlock);
    insert_modified_block(nextBlock);
    return 0;
}

/// @brief Get index of the next block
/// @param currentBlock
/// @return index
int FAT::get_next_block(int currentBlock) {
    return this->fat_array[currentBlock];
}

/// @brief Frees block in FAT array
/// @param index
void FAT::free_block(int index) {
    this->fat_array[index] = FAT_EOF;
    insert_modified_block(index);
}

/// @brief Write changes to disk
void FAT::save_on_disk() {
    char buffer[BLOCK_SIZE];

    /*
     *  goes over every modified block
     * start at the second entry since the first entry always point to the second modified
     * value and does not point anywhere if only one blockDevice is used
     */
    for(int i = 1; i < modifiedBlocksCounter; i++) {

        // check which position is allocated in blockdevice for FAT
        int block_device_offset = (modifiedBlocks[i - 1]) / (BLOCK_SIZE / 4);

        // read current blockdata
        blockDevice->read(FAT_OFFSET + block_device_offset, buffer);

        int current_index = modifiedBlocks[i];

        // watch from 0 to 512 iterations inside current blockDevice
        int current_iteration = 0;
        // write updated FAT, keep wathcing over the blockDevice offset
        for (int index = ((BLOCK_SIZE / 4) * block_device_offset); index < BLOCK_SIZE + (BLOCK_SIZE * current_index); index++) {

            // if current_iteration matches modified block
            if(index == modifiedBlocks[i - 1]) {
                // write each byte to buffer, start - left
                for(int j = 3; j >= 0; j--) {
                    // write next block pointer
                    int byte  = (current_index >> (8 * 1)) & 0xff;
                    int buffer_offset = (current_iteration * 4) + j;
                    buffer[buffer_offset] = byte;
                }
                break;
            }
            current_iteration++;
        }
        blockDevice->write(FAT_OFFSET + block_device_offset, buffer);
    }
    clear_modified_blocks();
}

///@brief Initialise the FAT(existing)
void FAT::init_fat() {
    char buffer[BLOCK_SIZE];
    for(int i = 0; i < FAT_SIZE; i++) {
        int offset = i + FAT_OFFSET;
        blockDevice->read(offset, buffer);
        memcpy(fat_array + (i * BLOCK_SIZE / 4), buffer, BLOCK_SIZE);
    }
}


///@brief Initialise the FAT
void FAT::first_init_fat() {
    char buffer[BLOCK_SIZE];
    for(int i = 0; i < FAT_SIZE; i++) {
        memcpy(buffer, fat_array, BLOCK_SIZE);
        blockDevice->write(FAT_OFFSET + i, buffer);
    }
}
