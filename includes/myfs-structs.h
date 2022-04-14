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

struct MyFsNode {
    char name[NAME_LENGTH]{};
    uint32_t size = 0;
    uid_t uid{};
    gid_t gid{};
    mode_t mode = S_IFREG | 0777;
    char* data{};
    time_t atime{}, ctime{}, mtime{};
};

struct BlockCache {
    char read_cache[BLOCK_SIZE]{};
    uint32_t read_cache_block = -1;
    char write_cache[BLOCK_SIZE]{};
    uint32_t write_cache_block = -1;
};


#endif /* myfs_structs_h */
