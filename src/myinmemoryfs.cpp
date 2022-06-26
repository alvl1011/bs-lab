//
// Created by Oliver Waldhorst on 20.03.20.
//  Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#include "myinmemoryfs.h"

// The functions fuseGettattr(), fuseRead(), and fuseReadDir() are taken from
// an example by Mohammed Q. Hussain. Here are original copyrights & licence:

/**
 * Simple & Stupid Filesystem.
 *
 * Mohammed Q. Hussain - http://www.maastaar.net
 *
 * This is an example of using FUSE to build a simple filesystem. It is a part of a tutorial in MQH Blog with the title
 * "Writing a Simple Filesystem Using FUSE in C":
 * http://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/
 *
 * License: GNU GPL
 */

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

/// @brief Constructor of the in-memory file system class.
///
/// You may add your own constructor code here.
MyInMemoryFS::MyInMemoryFS() : MyFS() {
    files = new MyFsNode[NUM_DIR_ENTRIES];
    open_files = new int[NUM_OPEN_FILES];
}

/// @brief Destructor of the in-memory file system class.
///
/// You may add your own destructor code here.
MyInMemoryFS::~MyInMemoryFS() {
    delete[] files;
    delete[] open_files;
}



/// @brief Create a new file.
///
/// Create a new file with given name and permissions.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode Permissions for file access.
/// \param [in] dev Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyInMemoryFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();
    try {
        if (get_index(path) != -1) {
            LOG("File exists.");
            RETURN(-EEXIST);
        }

        if (strlen(path) > NAME_LENGTH) {
            LOG("Filename is too long.");
            RETURN(-ENAMETOOLONG);
        }

        if (get_next_free_index() == -1) {
            LOG("No space left on device");
            RETURN(-ENOSPC);
        }

        auto *file = new MyFsNode();
        strcpy(file->name, path + 1); // + 1 for path '/'
        file->uid = getuid();
        file->gid = getgid();
        file->mode = mode;
        time(&(file->atime));
        time(&(file->ctime));
        time(&(file->mtime));
        files[get_next_free_index()] = *file;

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
int MyInMemoryFS::fuseUnlink(const char *path) {
    LOGM();
    try {
        int index = get_index(path);
        if (index == -1) {
            LOG("No such file or directory.");
            RETURN(-ENOENT);
        }

        strcpy(files[index].name, "\0");
        files[index].size = 0;
        files[index].mode = 0;
        files[index].uid = 0;
        files[index].gid = 0;
        files[index].atime = 0;
        files[index].ctime = 0;
        files[index].mtime = 0;
        free(files[index].data);

        LOG("File deleted successfully.");
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
int MyInMemoryFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    try {
        // if name length is valid
        if (strlen(newpath + 1) > NAME_LENGTH) {
            RETURN(-ENAMETOOLONG);
        }

        // if file already exists
        if (get_index(newpath) != -1) {
            RETURN(-EEXIST);
        }

        int index = get_index(path);
        if (index != -1) {
            strcpy(files[index].name, newpath + 1);
            LOG("File renamed.");
            RETURN(0);
        }
        LOG("No such file or directory.");
        RETURN(-ENOENT);
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
int MyInMemoryFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    // statbuf: Structure containing the meta data. cf. original code template

    LOGF( "\tAttributes of %s requested\n", path );

    // GNU's definitions of the attributes (http://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html):
    // 		st_uid: 	The user ID of the file’s owner.
    //		st_gid: 	The group ID of the file.
    //		st_atime: 	This is the last access time for the file.
    //		st_mtime: 	This is the time of the last modification to the contents of the file.
    //		st_mode: 	Specifies the mode of the file. This includes file type information (see Testing File Type) and
    //		            the file permission bits (see Permission Bits).
    //		st_nlink: 	The number of hard links to the file. This count keeps track of how many directories have
    //	             	entries for this file. If the count is ever decremented to zero, then the file itself is
    //	             	discarded as soon as no process still holds it open. Symbolic links are not counted in the
    //	             	total.
    //		st_size:	This specifies the size of a regular file in bytes. For files that are really devices this field
    //		            isn’t usually meaningful. For symbolic links this specifies the length of the file name the link
    //		            refers to.

    statbuf->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
    statbuf->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
    statbuf->st_atime = time( nullptr); // The last "a"ccess of the file/directory is right now
    statbuf->st_mtime = time( nullptr); // The last "m"odification of the file/directory is right now

    if ( strcmp( path, "/" ) == 0 )
    {
        statbuf->st_mode = S_IFDIR | 0755;
        statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
    }

    int index = get_index(path);
    int return_value = 0;

    if (strcmp(path, "/") == 0) {
        statbuf->st_mode = S_IFDIR | 0755;
        statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
    } else if (strcmp(path + 1, files[index].name) == 0) {
        statbuf->st_nlink = 1;
        statbuf->st_mode = files[index].mode;
        statbuf->st_size = files[index].size;
    }
    else {
        return_value = -ENOENT;
    }
    RETURN(return_value);
}

/// @brief Change file permissions.
///
/// Set new permissions for a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode New mode of the file.
/// \return 0 on success, -ERRNO on failure.
int MyInMemoryFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    try {
        int index = get_index(path);
        if (index == -1) {
            LOG("No such file or directory.");
            RETURN(-ENOENT);
        }

        files[index].mode = mode;

        LOGF("File permissions changed: %d\n", files[index].mode);
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
///
/// If gid is more than 100000 no group was given to chown so don't edit the edit the gid of the file
///
///
/// \param [in] path Name of the file, starting with "/".
/// \param [in] uid New user id.
/// \param [in] gid New group id.
/// \return 0 on success, -ERRNO on failure.
int MyInMemoryFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    try {
        int index = get_index(path);
        if (index == -1) {
            LOG("No such file or directory.");
            RETURN(-ENOENT);
        }

        files[index].uid = uid;

        if (gid <= 100000) {
            files[index].gid = gid;
        }

        LOGF("File owner changed: %d\n", files[index].mode);
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
int MyInMemoryFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    try {
        // if too many files are still opened
        if (open_files_count == NUM_OPEN_FILES) {
            LOGF("Too many open files. Open files count: %d\n", open_files_count);
            RETURN(-EMFILE);
        }

        int index_path = get_index(path);
        if (index_path == -1) {
            LOG("No such file or directory.");
            RETURN(-ENOENT);
        }

        int open_files_index = get_next_free_index_files();
        open_files[open_files_index] = index_path;
        open_files_count++;
        // update file handler
        fileInfo->fh = open_files_index;

        LOG("File opened.");
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
int MyInMemoryFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF( "--> Trying to read %s, %lu, %lu\n", path, (unsigned long) offset, size );
    try {
        // no such file or directory
        if(open_files[fileInfo->fh] == -ENOENT) {
            return -EBADF;
        }

        int index = open_files[fileInfo->fh];

        // if offset is bigger than file size
        if (offset >= files[index].size) {
            RETURN(0);
        }

        if ((offset + size) > files[index].size) {
            size = files[index].size - offset;
        }
        memcpy(buf, files[index].data + offset, size);
        time(&(files[index].atime));

        LOGF("File is read. Size: %lu", size);
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
int MyInMemoryFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    try {
        if(open_files[fileInfo->fh] == -ENOENT) {
            return -EBADF;
        }

        int index = open_files[fileInfo->fh];

        // define new file size as the max value of old size or new content
        int new_size = 0;
        if (files[index].size < (offset + size)) {
            new_size = offset + size;
        } else {
            new_size = files[index].size;
        }
        files[index].size = new_size;

        // change size of data to new size and write new contents
        files[index].data = (char*)(realloc(files[index].data, files[index].size + 1));
        memcpy(files[index].data + offset, buf, size);

        // update timestamps
        time(&(files[index].mtime));
        time(&(files[index].ctime));

        LOGF("File is written. Size: %lu", size);
        RETURN((int) size);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
        RETURN(-ENOSYS);
    }
}

/// @brief Close a file.
///
/// In Part 1 this includes decrementing the open file count.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] fileInfo Can be ignored in Part 1 .
/// \return 0 on success, -ERRNO on failure.
int MyInMemoryFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    try {
        int index = get_index(path);
        // if file exists?
        if (index != -1) {
            // look in open_files
            size_t i = 0;
            while (i < NUM_OPEN_FILES) {
                int curr_index = open_files[i];

                // if found file
                if (curr_index >= 0) {
                    if (strcmp(path + 1, files[curr_index].name) == 0) {
                        open_files[i] = -ENOENT;
                        open_files_count--;
                        break;
                    }
                }
                i++;
            }
        } else {
            RETURN(-ENOENT);
        }
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
int MyInMemoryFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    try {
        int index = get_index(path);
        int old_size = files[index].size;
        if (index == -1) {
            LOG("No such file or directory.");
            RETURN(-ENOENT);
        }
        int return_value = truncate(index, newSize);

        // if truncate responses '666' -> File is already with suitable size.
        if (return_value == 666) {
            RETURN(0);
        }
        LOGF("File size is set ot new size. Old size: %d; New size: %d", old_size, files[index].size);
        RETURN(return_value);
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
int MyInMemoryFS::fuseTruncate(const char *path, off_t newSize, struct fuse_file_info *fileInfo) {
    LOGM();
    try {
        int index = open_files[fileInfo->fh];
        int old_size = files[index].size;
        if (index == -1) {
            LOG("No such file or directory.");
            RETURN(-ENOENT);
        }
        int return_value = truncate(index, newSize);

        // if truncate responses '666' -> File is already with suitable size.
        if (return_value == 666) {
            RETURN(0);
        }

        LOGF("File size is set ot new size. Old size: %d; New size: %d", old_size, files[index].size);
        RETURN(return_value);
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
int MyInMemoryFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF( "--> Getting The List of Files of %s\n", path );

    filler( buf, ".", nullptr, 0 ); // Current Directory
    filler( buf, "..", nullptr, 0 ); // Parent Directory

    // iterate over dir
    if (strcmp(path, "/") == 0) {
        for (int i = 0; i < NUM_DIR_ENTRIES; i++) {
            if (files[i].name[0] != '\0' ) {
                LOGF("    %s\n", files[i].name);
                filler(buf, files[i].name, nullptr, 0);
            }
        }
    }

    RETURN(0);
}

/// Initialize a file system.
///
/// This function is called when the file system is mounted. You may add some initializing code here.
/// \param [in] conn Can be ignored.
/// \return 0.
void* MyInMemoryFS::fuseInit(struct fuse_conn_info *conn) {

    try {
        // Open logfile
        this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
        if(this->logFile == nullptr) {
            fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
        } else {
            // turn of logfile buffering
            setvbuf(this->logFile, nullptr, _IOLBF, 0);

            LOG("Starting logging...\n");

            LOG("Using in-memory mode");

            // initialize  arrays for tracking the files during runtime
            for (size_t i = 0; i < NUM_DIR_ENTRIES; i++) {
                files[i].name[0] = '\0';
            }
            for (int i = 0; i < NUM_OPEN_FILES; i++) {
                open_files[i] = -ENOENT;
            }

        }

        RETURN(0);
    } catch (const std::exception& e) {
        LOGF("Problem occurred: %s\n", e.what());
    }
}

/// @brief Clean up a file system.
///
/// This function is called when the file system is unmounted. You may add some cleanup code here.
void MyInMemoryFS::fuseDestroy() {
    LOGM();
}

// TODO: [PART 1] You may add your own additional methods here!

/// @brief  Returns the index of a given file
/// @param path - path of file
/// @return index or -1 if not found
int MyInMemoryFS::get_index(const char *path) const {
    // Ignore '/' at the beginning of file path
    path++;
    for (size_t i = 0; i < NUM_DIR_ENTRIES; i++) {
        if (files[i].name[0] != '\0' && strcmp(path, files[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

/// @brief Returns index of the next free slot for creating an item.
///
/// @return index or -1 if not found
///
int MyInMemoryFS::get_next_free_index() const {
    for (size_t i = 0; i < NUM_DIR_ENTRIES; i++) {
        if (files[i].name[0] == '\0') {
            return i;
        }
    }
    return -1;
}

/// @brief
/// Returns index of the next files
///   ENOENT : No such file or Directory code
/// @return index or -1 if not found
int MyInMemoryFS::get_next_free_index_files() const {
    for (size_t i = 0; i < NUM_OPEN_FILES; i++) {
        if (open_files[i] == -ENOENT) {
            return i;
        }
    }
    return -1;
}

/// @brief Shrink or extend the size of a file to the specified size
/// @param path - path of file
int MyInMemoryFS::truncate(uint16_t file_index, off_t new_size) const {

    // If file is already of the suitable size
    if (files[file_index].size == new_size) {
        LOG("File is already with suitable size.");
        RETURN(666);
    }
    uint32_t old_size = files[file_index].size;
    files[file_index].size = new_size;
    files[file_index].data = (char *) (realloc(files[file_index].data, new_size));

    // If new_size is bigger than old - fill it with '\0'
    if (new_size > old_size) {
        uint32_t diff = new_size - old_size;
        uint32_t offset = new_size - diff;
        for (size_t i = 0; i < diff; i++) {
            files[file_index].data[offset + i] = '\0';
        }
    }
    time(&(files[file_index].mtime));
    time(&(files[file_index].ctime));

    LOG("File is truncated.");
    RETURN(0);
}



// DO NOT EDIT ANYTHING BELOW THIS LINE!!!

/// @brief Set the static instance of the file system.
///
/// Do not edit this method!
void MyInMemoryFS::SetInstance() {
    MyFS::_instance= new MyInMemoryFS();
}


