//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#ifndef MYFS_MYINMEMORYFS_H
#define MYFS_MYINMEMORYFS_H

#include <fuse.h>
#include <cmath>

#include "myfs.h"
#include "blockdevice.h"
#include "myfs-structs.h"

/// @brief In-memory implementation of a simple file system.
class MyInMemoryFS : public MyFS {
protected:
    // BlockDevice blockDevice;

public:
    static MyInMemoryFS *Instance();

    MyFsNode *files;
    uint32_t *open_files;
    int open_files_count = 0;

    MyInMemoryFS();
    ~MyInMemoryFS() override;

    static void SetInstance();

    // --- Methods called by FUSE ---
    // For Documentation see https://libfuse.github.io/doxygen/structfuse__operations.html
    // Vova
    virtual int fuseGetattr(const char *path, struct stat *statbuf);
    virtual int fuseMknod(const char *path, mode_t mode, dev_t dev);
    virtual int fuseUnlink(const char *path);
    virtual int fuseRename(const char *path, const char *newpath);
    virtual int fuseChmod(const char *path, mode_t mode);
    virtual int fuseChown(const char *path, uid_t uid, gid_t gid);
    virtual int fuseTruncate(const char *path, off_t newSize);
    virtual int fuseOpen(const char *path, struct fuse_file_info *fileInfo);
    //Alex
    virtual int fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseRelease(const char *path, struct fuse_file_info *fileInfo);
    virtual void* fuseInit(struct fuse_conn_info *conn);
    virtual int fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo);
    virtual void fuseDestroy();

    // TODO: Add methods of your file system here

    uint16_t get_index(const char *path);
    uint16_t get_next_free_index();
    uint16_t get_next_free_index_files();
    uint16_t truncate(uint16_t file_index, off_t new_size);

};

#endif //MYFS_MYINMEMORYFS_H
