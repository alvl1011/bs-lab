//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

#include <sys/stat.h>
#include "blockdevice.h"
#include <cstring>
#include <unistd.h>
#include <ctime>

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES NUM_DIR_ENTRIES
#define DATA_BLOCKS 262144             // 134 MB total FS size

#define BD_SIZE 0

#define DMAP_OFFSET BD_SIZE  // if we want to add superblock later
#define DMAP_SIZE 512

#define FAT_OFFSET DMAP_OFFSET + DMAP_SIZE
#define FAT_SIZE 2048
#define FAT_EOF -1         // value of terminator, to make a last block of a file

#define ROOT_DIR_OFFSET FAT_OFFSET + FAT_SIZE
#define ROOT_DIR_SIZE NUM_DIR_ENTRIES

#define DATA_OFFSET ROOT_DIR_OFFSET + ROOT_DIR_SIZE // offset for actual data block
#define TOTAL_BLOCKS DMAP_SIZE + FAT_SIZE + ROOT_SIZE + DATA_BLOCKS // define total amount of blocks

// this becomes obsolete for the myfsondisk as the data pointer
struct MyFsNode {
    char name[NAME_LENGTH]{};
    int size = 0;
    uid_t uid;
    gid_t gid;
    mode_t mode = S_IFREG | 0777;
    char* data;
    time_t atime, ctime, mtime;
};

// root block - one Block as struct to store metadata
struct RootFile {
    char name[NAME_LENGTH]{};
    struct stat stat = {}; // store metadata
    int firstBlock; // index of first data block
    int rootDirBlock; // index of data block for metadata
};

// Info on opened files
struct BlockCache {
    char read_cache[BLOCK_SIZE];
    uint32_t read_cache_block = -1;
    char write_cache[BLOCK_SIZE];
    uint32_t write_cache_block = -1;
    RootFile *rootFile;
};


#endif /* myfs_structs_h */
