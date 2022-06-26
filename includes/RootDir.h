//
// Created by Vladimir on 06.06.2022.
//

#ifndef MYFS_ROOTDIR_H
#define MYFS_ROOTDIR_H

#include <Utils.h>
#include "myfs-structs.h"

class RootDir {
private:
    BlockDevice *blockDevice;
    RootFile* existing_files[NUM_DIR_ENTRIES]{};
    int existing_files_counter = 0;

public:
    RootDir(BlockDevice *device);
    ~RootDir();

    RootFile* create_file(const char *path, mode_t mode);
    void delete_file(RootFile *file);

    RootFile* get_file(const char *path);
    RootFile** get_files();

    RootFile* load(int index);

    void save_on_disk(RootFile *file);

    void init_root_dir();
    void first_init_root_dir();

    bool save_on_disk();
};

#endif //MYFS_ROOTDIR_H
