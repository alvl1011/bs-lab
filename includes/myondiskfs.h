//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#ifndef MYFS_MYONDISKFS_H
#define MYFS_MYONDISKFS_H

#include "myfs.h"
#include "RootDir.h"
#include "DMAP.h"
#include "FAT.h"

/// @brief On-disk implementation of a simple file system.
class MyOnDiskFS : public MyFS {
protected:
     BlockDevice *blockDevice;
     char *buffer;
     RootDir *rootDir;
     DMAP *dmap;
     FAT *fat;

     int open_files_count = 0;
     BlockCache *open_files[NUM_OPEN_FILES];

public:
    static MyOnDiskFS *Instance();

    MyOnDiskFS();
    ~MyOnDiskFS();

    static void SetInstance();

    // --- Methods called by FUSE ---
    // For Documentation see https://libfuse.github.io/doxygen/structfuse__operations.html
    virtual int fuseGetattr(const char *path, struct stat *statbuf);
    virtual int fuseMknod(const char *path, mode_t mode, dev_t dev);
    virtual int fuseUnlink(const char *path);
    virtual int fuseRename(const char *path, const char *newpath);
    virtual int fuseChmod(const char *path, mode_t mode);
    virtual int fuseChown(const char *path, uid_t uid, gid_t gid);
    virtual int fuseTruncate(const char *path, off_t newSize);
    virtual int fuseOpen(const char *path, struct fuse_file_info *fileInfo);
    virtual int fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseRelease(const char *path, struct fuse_file_info *fileInfo);
    virtual void* fuseInit(struct fuse_conn_info *conn);
    virtual int fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo);
    virtual void fuseDestroy();

    int get_next_free_index_opened_files();
    int read_file(int *blocks, int block_count, int offset, size_t size, char *buf, BlockCache *open_file);
    int write_file(int *blocks, int block_count, int offset, size_t size, const char *buffer, BlockCache *open_file);
};

#endif //MYFS_MYONDISKFS_H
