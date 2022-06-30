//
// Created by Vladimir on 10.06.2022.
//

#include <DMAP.h>

/// @brief DMAP constructor
/// init blockDevice for current object
/// @param blockDevice

DMAP::DMAP(BlockDevice *blockDevice) {
    this->blockDevice = blockDevice;
    for (int i = 0; i < DATA_BLOCKS; i++) {
        this->blocks[i] = false;
    }
}


/// @brief Destructor of DMAP
DMAP::~DMAP() = default;


/// Get next free block available
/// @return int free-block or -1 if free-block not found
int DMAP::get_next_free_block() {
    for(int i = 0; i < DATA_BLOCKS; i++) {
        if(!get_block_state(i)) {
            set_block_state(i, true); //set new state of block
            return i;
        }
    }
    return -1;
}

/// @brief Get array of indexes of set amount of data blocks
/// @param amount
/// @return int[amount]
int* DMAP::get_amount_free_blocks(int amount) {
    int* free_blocks = new int[amount];

    for(size_t i = 0; i < amount; i++) {
        int free_index = get_next_free_block();
        if(free_index < 0) {
            return nullptr;
        }
        free_blocks[i] = free_index;
    }
    return free_blocks;
}

/// @brief Set usage status of data block to true or false if in use or not

void DMAP::set_block_state(int dataBlockNumber, bool isUsed) {
    this->blocks[dataBlockNumber] = isUsed;
    if(isUsed) decrease_free_block_counter_by(1);
    else increase_free_block_counter_by(1);
}

/// @brief Get the usage status of data block
/// @return true|false
bool DMAP::get_block_state(int dataBlockNumber) {
    return this->blocks[dataBlockNumber];
}

/// Increase the counter which keeps track of the total of free blocks by
/// the requested amount
void DMAP::increase_free_block_counter_by(int amount) {
    this->blockInUseCounter += amount;
}

/// Decrease the counter which keeps track of the total of free blocks by
/// the requested amount
void DMAP::decrease_free_block_counter_by(int amount) {
    this->blockInUseCounter -= amount;
}

/// @brief Write changes to disk
bool DMAP::save_on_disk() {
    char buffer[BLOCK_SIZE];

    // for blocks which need to be stores with DMAP
    for (int current_index = 0; current_index < DMAP_SIZE; current_index++) {
        // store one bit for every written block
        for (int byte_index = 0; byte_index < BLOCK_SIZE; byte_index++) {

            // index of available blocks
            int blocks_track_index = (current_index * BLOCK_SIZE) + byte_index;

            /*
             * verify that the index from the array which tracks all available blocks
             * is within bounds
             * */
            if(blocks_track_index < DATA_BLOCKS)
                buffer[byte_index] = blocks[blocks_track_index];
            else
                buffer[byte_index] = 0;
        }
        this->blockDevice->write(DMAP_OFFSET + current_index, buffer);
    }
    return true;
}

/// @brief Initialise the DMAP(existing) and check current available blocks
void DMAP::init_dmap() {

    char buffer[BLOCK_SIZE];

    // iterate over every block assigned to the DMAP
    for (int i = 0; i < DMAP_SIZE; i++) {

        // set block index if superblock has been defined
        int current_index = i + DMAP_OFFSET;

        // reset buffer
        memset(buffer, 0, BLOCK_SIZE);

        // get the block data
        this->blockDevice->read(current_index, buffer);

        //assign each value from buffer to array of blocks
        for(int j = 0; j < BLOCK_SIZE; j++) {
            int blocks_index = (BLOCK_SIZE * i) + j;
            // check if blocks index is in range
            if(blocks_index < DATA_BLOCKS) {
                // init blocks array
                this->blocks[blocks_index] = buffer[j];
                if (buffer[j] == 1) decrease_free_block_counter_by(1);
            }
        }
    }
}

/// @brief Initialize for empty file system
void DMAP::first_init_dmap() {
    char buffer[BLOCK_SIZE];
    for(int i = 0; i < DMAP_SIZE; i++) {
        memset(buffer, 0, BLOCK_SIZE);
        blockDevice->write(DMAP_OFFSET + i, buffer);
    }
}
