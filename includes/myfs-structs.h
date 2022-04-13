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

#define DATA_SIZE 65536
#define FILE_SIZE 288

#define SUPERBLOCK_FIRST_BLOCK 0
#define ROOT_START 384
#define DATA_START 420

#define DMAP_SIZE 16
#define FAT_SIZE 256

// TODO: Add structures of your file system here

typedef struct {
    uint16_t magic_address;
    char* block[BLOCK_SIZE];
} super_block;

typedef struct {
    char* index[BLOCK_SIZE * DMAP_SIZE];
} DMAP;

typedef struct {
    char* index[BLOCK_SIZE * FAT_SIZE];
} FAT;

typedef struct {
    char* data[BLOCK_SIZE];
} DATA;


typedef struct {
    char name[NAME_LENGTH + 1];
    uint32_t size;
    char* data;
    uid_t uid;
    gid_t gid;
    mode_t mode;
    time_t atime;
    time_t ctime;
    time_t mtime;
    uint16_t first_block_id;
} dir;


struct file {
    char name[NAME_LENGTH + 1]{};
    uint16_t uid;
    uint16_t gid;
    uint32_t size;
    mode_t mode;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t atime;
    char* data;

    file() {
        name[0] = '\0';
        size = 0;
        data = nullptr;
        uid = 0;
        gid = 0;
        mode = 0;
        atime = 0;
        ctime = 0;
        mtime = 0;
    }
};
#endif /* myfs_structs_h */
