//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

#define FS_BLOCK_SIZE 1024
#define FS_MAGIC 0x37363030

// TODO: Add structures of your file system here

/**
 * @brief SUPERBLOCK
 *
 * The superblock is the first block in the file system,
 * and contains the information needed to find
 * the rest of the file system structures.
 *
 * Structure:
 *      magic_number - magic number - address
 *      inode_map_size - inode map size in blocks
 *      inode_region_size - inode region size in blocks
 *      block_map_size - block map size in blocks
 *      num_of_blocks - total blocks, including SUPERBLOCK, bitmaps, inodes
 *      root_node - always inode 1, reserved for SUPERBLOCK
 *
 *  Pad out to the entire block
 *      pad - total FS_BLOCK size
 *
 */
struct SUPERBLOCK
{
    uint32_t magic_number;
    uint32_t inode_map_size;
    uint32_t inode_region_size;
    uint32_t block_map_size;
    uint32_t num_of_blocks;
    uint32_t root_node;
    char pad[FS_BLOCK_SIZE - 6 * sizeof(uint32_t)];
};

struct FS_INODE
{
    uint16_t uid;
    uint16_t gid;
    uint32_t mode;
    uint32_t c_time;
    uint32_t m_time;
    int32_t size;
    uint32_t indir_1;
    uint32_t indir_2;
};

#endif /* myfs_structs_h */
