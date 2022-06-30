//
// Created by Vladimir on 10.06.2022.
//

#include <Utils.h>

/// @brief Goes over a buffer to go over all of the block bits and check if content is zero
/// @param buffer
/// @return true|false - if the block is empty - full of zeros or not
bool Utils::checkContent(char buffer[]) {

    // goes over every byte from blockDevice
    for (int char_index = 0; char_index < BLOCK_SIZE; char_index++) {
        // check bits of given byte
        for (int bit_index = 0; bit_index < 8; bit_index++) {
            // if anything is written, block is not free - return false;
            if((buffer[char_index] & (1 << (7 - bit_index))) == 1)
                return false;
        }
    }
    return true;
}

