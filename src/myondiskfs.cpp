//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#include "myondiskfs.h"

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment lines to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <exception>

#include "macros.h"
#include "myfs.h"
#include "myfs-info.h"
#include "blockdevice.h"

/// @brief Constructor of the on-disk file system class.
///
/// You may add your own constructor code here.
MyOnDiskFS::MyOnDiskFS() : MyFS() {
    // create a block device object
    this->blockDevice= new BlockDevice(BLOCK_SIZE);

    buffer = new char[BLOCK_SIZE]; // For blocks
    dmap = new DMAP(blockDevice); // structure for checking if block is free or used
    fat = new FAT(blockDevice); // next block location
    rootDir = new RootDir(blockDevice); // metadata of files
}

/// @brief Destructor of the on-disk file system class.
///
/// You may add your own destructor code here.
MyOnDiskFS::~MyOnDiskFS() {
    // free block device object
    delete this->blockDevice;
    delete rootDir;
    delete fat;
    delete dmap;
}

/// @brief Create a new file.
///
/// Create a new file with given name and permissions.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode Permissions for file access.
/// \param [in] dev Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();
    try {
        if (strlen(path) > NAME_LENGTH) {
            LOG("Filename is too long.");
            RETURN(-ENAMETOOLONG);
        }

        if (get_next_free_index_opened_files() == -1) {
            LOG("No space left on device");
            RETURN(-ENOSPC);
        }

        RootFile *file = rootDir->create_file(path, mode); // pointer to new file
        this->rootDir->save_on_disk(file); // write file into container

        LOG("File created successfully.");
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Delete a file.
///
/// Delete a file with given name from the file system.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseUnlink(const char *path) {
    LOGM();
    try {

        RootFile *file = rootDir->get_file(path); // get structure of file

        if (file == nullptr) {
            LOG("File does not exist.");
            RETURN(-ENOENT);
        }

        if (file->firstBlock != -1) {
            int current_block = file->firstBlock;
            int next_block = FAT_EOF; // For loop, since outwise it would run forever
            do {
                // get next block
                next_block = fat->get_next_block(current_block);
                // set bit to 0 to represent empty space
                dmap->set_block_state(current_block, false);
                // delete block
                fat->free_block(current_block);
                // continue until fat is not at the end
                current_block = next_block;

            } while (next_block != FAT_EOF);
        }

        dmap->save_on_disk(); // write to block device
        fat->save_on_disk();

        // clear metadata
        char local_buffer[BLOCK_SIZE];
        memset(local_buffer, 0, BLOCK_SIZE);
        this->blockDevice->write(ROOT_DIR_OFFSET + file->rootDirBlock, local_buffer);

        // clear file from rootdir
        rootDir->delete_file(file);
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Rename a file.
///
/// Rename the file with with a given name to a new name.
/// Note that if a file with the new name already exists it is replaced (i.e., removed
/// before renaming the file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newpath  New name of the file, starting with "/".
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    try {

        // name length
        if(strlen(newpath + 1) > NAME_LENGTH) {
            RETURN(-ENAMETOOLONG);
        }

        RootFile *file = rootDir->get_file(path);
        // if file does not exist
        if (file == nullptr) {
            RETURN(-ENOENT);
        }

        // if file exists
        if(rootDir->get_file(newpath) != nullptr) {
            RETURN(-EEXIST);
        }

        strcpy(file->name, newpath + 1);
        rootDir->save_on_disk(file); // save new file, filename changed

        LOG("File renamed successfully.");
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Get file meta data.
///
/// Get the metadata of a file (user & group id, modification times, permissions, ...).
/// \param [in] path Name of the file, starting with "/".
/// \param [out] statbuf Structure containing the meta data, for details type "man 2 stat" in a terminal.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    try {

        // get metadata from old folder
        if(strcmp(path, "/") == 0) {
            statbuf->st_mode = S_IFDIR | 0755;
            statbuf->st_nlink = 2;
            statbuf->st_uid = getuid();
            statbuf->st_gid = getgid();
            statbuf->st_atime = time(nullptr);
            statbuf->st_mtime = time(nullptr);
            RETURN(0);
        }

        // if requested path is not root "/" get file through path
        RootFile *file = rootDir->get_file(path);

        if(file == nullptr) {
            RETURN(-ENOENT);
        }
        // copy new struct into buffer
        memcpy(statbuf, &file->stat, sizeof(*statbuf));
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Change file permissions.
///
/// Set new permissions for a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode New mode of the file.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    try {
        RootFile *file = rootDir->get_file(path);

        if (file == nullptr) {
            RETURN(-ENOENT);
        }

        file->stat.st_mode = mode;
        rootDir->save_on_disk(file);

        LOG("File mode changed successfully.");
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Change the owner of a file.
///
/// Change the user and group identifier in the meta data of a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] uid New user id.
/// \param [in] gid New group id.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    try {
        RootFile *file = rootDir->get_file(path);

        if (file == nullptr) {
            RETURN(-ENOENT);
        }

        file->stat.st_uid = uid;
        file->stat.st_gid = gid;
        rootDir->save_on_disk(file);

        LOG("User and group identifier changed successfully.");
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Open a file.
///
/// Open a file for reading or writing. This includes checking the permissions of the current user and incrementing the
/// open file count.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [out] fileInfo Can be ignored in Part 1
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    try {
        // if other open file is allowed
        if (open_files_count == NUM_OPEN_FILES) {
            RETURN(-EMFILE);
        }

        RootFile *file = rootDir->get_file(path);

        if (file == nullptr) {
            RETURN(-ENOENT);
        }

        open_files_count++;

        int open_file_index = get_next_free_index_opened_files();

        // create a new open file object from file specified by the path
        auto *open_file = new ::BlockCache();
        open_file->rootFile = file;

        // add to open files
        open_files[open_file_index] = open_file;

        // update index in fuse file object
        fileInfo->fh = open_file_index;

        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Read from a file.
///
/// Read a given number of bytes from a file starting form a given position.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// Note that the file content is an array of bytes, not a string. I.e., it is not (!) necessarily terminated by '\0'
/// and may contain an arbitrary number of '\0'at any position. Thus, you should not use strlen(), strcpy(), strcmp(),
/// ... on both the file content and buf, but explicitly store the length of the file and all buffers somewhere and use
/// memcpy(), memcmp(), ... to process the content.
/// \param [in] path Name of the file, starting with "/".
/// \param [out] buf The data read from the file is stored in this array. You can assume that the size of buffer is at
/// least 'size'
/// \param [in] size Number of bytes to read
/// \param [in] offset Starting position in the file, i.e., number of the first byte to read relative to the first byte of
/// the file
/// \param [in] fileInfo Can be ignored in Part 1
/// \return The Number of bytes read on success. This may be less than size if the file does not contain sufficient bytes.
/// -ERRNO on failure.
int MyOnDiskFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    try {
        // check whether there is an open file
        if (open_files[fileInfo->fh] == nullptr) {
            LOG("Bad file number");
            RETURN(-EBADF);
        }

        // if is opened write it to the root file obj
        RootFile *file = open_files[fileInfo->fh]->rootFile;

        // Nothing to read because user wants to read outside of file content
        // Example: file has 5 lines, user wants to start to read at line 6
        if (offset >= file->stat.st_size) {
            RETURN(0);
        }

        // offset is in range but size (amount of bytes to read) is too big,
        // we only want to read in the defined file and not outside
        // Example: file has 5 lines, user wants to read lines 2 to 6. User only gets to read lines 2 to 5
        if ((off_t)(offset + size) > file->stat.st_size) {
            size = file->stat.st_size - offset;
        }

        // number of blocks to read
        int block_count;

        // calculate number of blocks to read
        if (size % BLOCK_SIZE == 0) {
            // file size is exactly multiple of BLOCK_SIZE
            block_count = (int) size / BLOCK_SIZE;
        }
        else {
            // +1 to read the next block too
            block_count = (int) size / BLOCK_SIZE + 1;
        }

        // if offset is bigger than BLOCK_SIZE we read some block in the chain,
        // determine how many blocks we need to skip
        int block_offset = (int) offset / BLOCK_SIZE;

        // skip blocks
        int currentBlock = file->firstBlock;
        for (int i = 0; i < block_offset; i++) {
            currentBlock = fat->get_next_block(currentBlock);
        }

        // construct chain of blocks that need to be read
        int *blocks = new int[block_count];
        blocks[0] = currentBlock;
        for (int i = 1; i < block_count; i++) {
            //get the next block from FAT
            currentBlock = fat->get_next_block(currentBlock);
            blocks[i] = currentBlock;
        }

        // read file
        int err = read_file(blocks, block_count, (int) offset % BLOCK_SIZE, size, buf, open_files[fileInfo->fh]);

        // update file because of the new a_time
        file->stat.st_atime = time(nullptr);

        // update a_time in the rootDir
        this->rootDir->save_on_disk(file);

        // we do not want to store temp blocks
        delete[] blocks;

        // return amount of bytes to read
        RETURN((int) size);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Write to a file.
///
/// Write a given number of bytes to a file starting at a given position.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// Note that the file content is an array of bytes, not a string. I.e., it is not (!) necessarily terminated by '\0'
/// and may contain an arbitrary number of '\0'at any position. Thus, you should not use strlen(), strcpy(), strcmp(),
/// ... on both the file content and buf, but explicitly store the length of the file and all buffers somewhere and use
/// memcpy(), memcmp(), ... to process the content.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] buf An array containing the bytes that should be written.
/// \param [in] size Number of bytes to write.
/// \param [in] offset Starting position in the file, i.e., number of the first byte to read relative to the first byte of
/// the file.
/// \param [in] fileInfo Can be ignored in Part 1 .
/// \return Number of bytes written on success, -ERRNO on failure.
int MyOnDiskFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    try {

        // is file opened
        if (open_files[fileInfo->fh] == nullptr) {
            RETURN(-EBADF);
        }
        RootFile *file = open_files[fileInfo->fh]->rootFile;

        // collect info about how many and which blocks
        // use to store data to be written

        // number of blocks to write
        int block_count;
        if (size % BLOCK_SIZE == 0)
            block_count = (int) size / BLOCK_SIZE;
        else
            block_count = (int) size / BLOCK_SIZE + 1;

        // offset block for starting writing on existing files
        int block_offset = (int) offset / BLOCK_SIZE;
        // if existing files is get bigger append to existing block
        int delta_count_block = block_offset + block_count - file->stat.st_blocks;

        // if we need new block
        if (delta_count_block > 0) {
            // create array of free blocks according to size of data
            // mark blocks as used
            int *new_blocks = dmap->get_amount_free_blocks(block_count);

            // is file new or empty
            if (file->firstBlock == -1)
                file->firstBlock = new_blocks[0];
            else {
                // editing existing file
                // append last block of file to new_blocks
                int current_block = file->firstBlock;
                // get end of chain
                while (fat->get_next_block(current_block) != FAT_EOF)
                    current_block = fat->get_next_block(current_block);
                fat->set_next_block(current_block, new_blocks[0]);
            }

            // store chain of blocks in FAT
            for (size_t i = 0; i < block_count; i++)
                fat->set_next_block(new_blocks[i - 1], new_blocks[i]);

            delete[] new_blocks;
            file->stat.st_blocks = block_count + block_offset;
        }

        // write size of new file
        if ((off_t)(offset + size) > file->stat.st_size)
            file->stat.st_size = offset + size;

        int current_block = file->firstBlock;

        // move to offset in case of writing to existing file
        for (size_t i = 0; i <= block_offset; i++) {
            int next_block_index = fat->get_next_block(current_block);
            if (next_block_index != FAT_EOF)
                current_block = next_block_index;
        }

        // collect block for writing
        int *blocks = new int[block_count];
        blocks[0] = current_block;
        for (size_t i = 0; i < delta_count_block; i++) {
            current_block = fat->get_next_block(current_block);
            blocks[i] = current_block;
        }

        // pass in function to write blocks
        int written = write_file(blocks, block_count, offset % BLOCK_SIZE, size, const_cast<char *>(buf), open_files[fileInfo->fh]);

        if (written != 0) {
            RETURN(-EIO);
        }

        // set new timestamps
        file->stat.st_mtime = time(nullptr);
        file->stat.st_ctime = time(nullptr);

        // save on disk
        dmap->save_on_disk();
        fat->save_on_disk();
        rootDir->save_on_disk(file);

        LOG("File was written.");
        RETURN((int) size);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Close a file.
///
/// \param [in] path Name of the file, starting with "/".
/// \param [in] File handel for the file set by fuseOpen.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    try {
        int open_file_index = fileInfo->fh;
        delete open_files[open_file_index];
        open_files[open_file_index] = nullptr;
        open_files_count--;

        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Truncate a file.
///
/// Set the size of a file to the new size. If the new size is smaller than the old size, spare bytes are removed. If
/// the new size is larger than the old size, the new bytes may be random.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newSize New size of the file.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    try {
        // if new size is 0 truncate removes file

        // truncate 'only' works because fuseWrite is called by fuse afterwards.
        // fuseTruncate basically just takes care of the fat/dmap handling after removing/adding more data
        struct fuse_file_info fileInfo = {};

        // open the file: return errno - or - 0 if successful
        int ret = fuseOpen(path, &fileInfo);

        if (ret != 0) {
            RETURN(ret);
        }
        // resize file
        ret = fuseTruncate(path, newSize, &fileInfo);

        if (ret != 0) {
            RETURN(ret);
        }
        // Close opened file
        ret = fuseRelease(path, &fileInfo);
        // Return final error code - or - 0 on success
        LOG("File was truncated.");
        RETURN(ret);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Truncate a file.
///
/// Set the size of a file to the new size. If the new size is smaller than the old size, spare bytes are removed. If
/// the new size is larger than the old size, the new bytes may be random. This function is called for files that are
/// open.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newSize New size of the file.
/// \param [in] fileInfo Can be ignored in Part 1.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseTruncate(const char *path, off_t newSize, struct fuse_file_info *fileInfo) {
    LOGM();
    try {

        // check if correct file is opened
        if (open_files[fileInfo->fh] == nullptr) {
            RETURN(-EBADF);
        }
        // put file information in rootFile structure
        RootFile *file = open_files[fileInfo->fh]->rootFile;

        // if file size is not changed return 0
        if (file->stat.st_size == newSize) {
            RETURN(0);
        }

        uint32_t block_count;
        // count required amount of blocks
        if (newSize % BLOCK_SIZE == 0)
            block_count = newSize / BLOCK_SIZE;
        else
            block_count = newSize / BLOCK_SIZE + 1;

        // count the difference in blocks (to be removed later on)
        uint32_t delta_block = file->stat.st_blocks - block_count;

        // should always be >= 1 unless we are wiping file
        if (delta_block > 0) {

            uint32_t offset_block = file->stat.st_blocks - delta_block;
            int32_t current_block = file->firstBlock;

            // move to offset block
            for (size_t i = 1; i < offset_block; i++) {
                current_block = fat->get_next_block(current_block);
            }

            int32_t next_block = fat->get_next_block(current_block);

            // go through dmap and fat chain and set them all to unused
            while (next_block != FAT_EOF) {
                fat->free_block(current_block);
                dmap->set_block_state(next_block, false);
                current_block = next_block;
                next_block = fat->get_next_block(current_block);
            }

            if (block_count == 0) {
                // if file was truncated to zero starting block was not freed yet
                dmap->set_block_state(file->firstBlock, false);
                file->firstBlock = -1;
            }
            file->stat.st_blocks = block_count;
        }

        // metadata
        file->stat.st_size = newSize;
        file->stat.st_mtime = time(nullptr);
        file->stat.st_ctime = time(nullptr);

        // save on disk
        dmap->save_on_disk();
        fat->save_on_disk();
        rootDir->save_on_disk(file);

        LOG("File was truncated.");
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Read a directory.
///
/// Read the content of the (only) directory.
/// You do not have to check file permissions, but can assume that it is always ok to access the directory.
/// \param [in] path Path of the directory. Should be "/" in our case.
/// \param [out] buf A buffer for storing the directory entries.
/// \param [in] filler A function for putting entries into the buffer.
/// \param [in] offset Can be ignored.
/// \param [in] fileInfo Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    try {
        // Dir self reference
        filler(buf, ".", nullptr, 0);
        // Parent dir reference
        filler(buf, "..", nullptr, 0);

        // Since we have one root directory we do not have to check for a directory
        RootFile **files = rootDir->get_files();
        for (size_t i = 0; i < NUM_DIR_ENTRIES; i++) {
            if(files[i] != nullptr) {
                struct stat s = {};
                fuseGetattr(files[i]->name, &s);
                filler(buf, files[i]->name, &s, 0);
            }
        }
        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// Initialize a file system.
///
/// This function is called when the file system is mounted. You may add some initializing code here.
/// \param [in] conn Can be ignored.
/// \return 0.
void* MyOnDiskFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
    if(this->logFile == nullptr) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
    } else {
        // turn of logfile buffering
        setvbuf(this->logFile, nullptr, _IOLBF, 0);

        LOG("Starting logging...\n");

        LOG("Using on-disk mode");

        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        int ret= this->blockDevice->open(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        if(ret >= 0) {
            LOG("Container file does exist, reading");

            // TODO: [PART 2] Read existing structures form file

        } else if(ret == -ENOENT) {
            LOG("Container file does not exist, creating a new one");

            ret = this->blockDevice->create(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

            if (ret >= 0) {
                LOG("Container file exists, reading...");

                dmap->init_dmap();
                fat->init_fat();
                rootDir->init_root_dir();
            } else if(ret == -ENOENT) {
                LOG("Container file does not exist, creating a new one...");

                ret = this->blockDevice->create(((MyFsInfo *)fuse_get_context()->private_data)->contFile);

                if (ret >= 0) {
                    dmap->first_init_dmap();
                    fat->first_init_fat();
                    rootDir->first_init_root_dir();
                }
            }
        }

        if(ret < 0) {
            LOGF("ERROR: Access to container file failed with error %d", ret);
        }
     }

    RETURN(NULL);
}

/// @brief Clean up a file system.
///
/// This function is called when the file system is unmounted. You may add some cleanup code here.
void MyOnDiskFS::fuseDestroy() {
    LOGM();
}

// TODO: [PART 2] You may add your own additional methods here!

// DO NOT EDIT ANYTHING BELOW THIS LINE!!!

/// @brief Set the static instance of the file system.
///
/// Do not edit this method!
void MyOnDiskFS::SetInstance() {
    MyFS::_instance= new MyOnDiskFS();
}

/// @brief Check if the next index in the File-block is open.
int MyOnDiskFS::get_next_free_index_opened_files() {
    for (int i = 0; i < NUM_OPEN_FILES; i++)
    {
        if (open_files[i] == nullptr)
        {
            return i;
        }
    }
    return -1;
}

/// @param [*blocks] is an array of blocks with data
/// @param [block_count] how many blocks are in *blocks
/// @param [size] how many bytes do we need to write
/// @param [offset] offset of write method
/// @param [buffer] bytes to write on a disk
/// @param [open_file] block cache of opened file
/// @brief Method to read file from disk
/// @return 0 on success, -1 on failure
int MyOnDiskFS::read_file(int *blocks, int block_count, int offset, size_t size, char *buffer, BlockCache *open_file) {

    // Read all blocks by block counter
    for (int i = 0; i < block_count; i++)
    {

        // make sure the buffer is not filled with old stuff
        memset(this->buffer, 0, BLOCK_SIZE);

        // is the data that needs to be read available in cache?
        if (i == 0 && open_file->read_cache_block == (int)blocks[0])
            memcpy(this->buffer, open_file->read_cache, BLOCK_SIZE);
        else
            blockDevice->read(DATA_OFFSET + blocks[i], this->buffer);

        size_t buffer_offset = i * BLOCK_SIZE;
        size_t tmp_size;

        // first block
        if (i == 0) {
            // check if blocks[0] fits in current block
            if ((size_t)(offset + size) < (size_t)(BLOCK_SIZE - offset))
                tmp_size = size;
            else
                tmp_size = BLOCK_SIZE - offset;

            memcpy(buffer, this->buffer + offset, size);
        } else {
            // if remaining data fits in one block
            if (size < BLOCK_SIZE)
                tmp_size = size;
            else
                tmp_size = BLOCK_SIZE;
            memcpy(buffer + buffer_offset, this->buffer, tmp_size);
        }
        // update remaining bytes to read
        size -= tmp_size;

        // last block
        if (i == block_count - 1) {
            // store in cache
            open_file->read_cache_block = blocks[i];
            memcpy(open_file->read_cache, this->buffer, BLOCK_SIZE);
        }
    }

    // if file could not be read correctly and size is not set to 0 return error
    if (size != 0)
        return -1;
    RETURN(0);
}

/// @param [*blocks] is an array of blocks with data
/// @param [block_count] how many blocks are in *blocks
/// @param [size] how many bytes do we need to write
/// @param [offset] offset of write method
/// @param [buffer] bytes to write on a disk
/// @param [open_file] block cache of opened file
/// @brief Method to write file on disk
/// @return 0 on success, -1 on failure
int MyOnDiskFS::write_file(int *blocks, int block_count, int offset, size_t size, char *buffer, BlockCache *open_file) {

    // write all blocks by counter
    for (size_t i = 0; i < block_count; i++) {
        memset(this->buffer, 0, BLOCK_SIZE);
        // put already given data inside block into buffer
        if (i == 0 && open_file->write_cache_block == (int32_t) blocks[i]) {
            memcpy(this->buffer, open_file->write_cache, BLOCK_SIZE);
        } else {
            blockDevice->read(DATA_OFFSET + blocks[i], this->buffer);
        }

        size_t buffer_offset = i * BLOCK_SIZE;
        size_t tmp_size;

        // first block
        if (i == 0) {
            // if enough space in this block for the data
            if ((size_t)(offset + size) < (size_t)(BLOCK_SIZE))
                tmp_size = size;
            else
                tmp_size = BLOCK_SIZE - offset;
            // remaining space in this block
            memcpy(this->buffer + offset, buffer, tmp_size);
        } else {
            // if remaining data is smaller than BLOCK_SIZE
            if (size < BLOCK_SIZE)
                tmp_size = size;
            else
                tmp_size = BLOCK_SIZE;
            memcpy(this->buffer, buffer + buffer_offset, tmp_size);
        }

        // store new size
        size -= tmp_size;

        // last block
        if (i == block_count - 1) {
            open_file->write_cache_block = blocks[i];
            memcpy(open_file->write_cache, this->buffer, BLOCK_SIZE);
        }

        blockDevice->write(blocks[i] + DATA_OFFSET, this->buffer);
    }
    RETURN(0);
}
