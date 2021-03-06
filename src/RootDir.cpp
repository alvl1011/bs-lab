//
// Created by Vladimir on 10.06.2022.
//

#include <RootDir.h>

/// @brief RootDir - constructor
/// @param device
RootDir::RootDir(BlockDevice *blockDevice) {
    this->blockDevice = blockDevice;
    for(int i = 0; i < NUM_DIR_ENTRIES; i++)
        existing_files[i] = nullptr;
}

/// @brief RootDir destructor
RootDir::~RootDir() = default;

/// @brief Create file
/// @param path
/// @return RootFile*
RootFile* RootDir::create_file(const char *path) {
    path++;// due to slash

    int index = 0;
    while (existing_files[index] != nullptr)
        index++;

    RootFile *file = new RootFile();

    strcpy(file->name, path);
    file->stat.st_mode = S_IFREG | 0644;
    file->stat.st_blksize = BLOCK_SIZE;
    file->stat.st_size = 0;
    file->stat.st_blocks = 0;
    file->stat.st_nlink = 1;
    file->stat.st_atime = time(nullptr);
    file->stat.st_mtime = time(nullptr);
    file->stat.st_ctime = time(nullptr);
    file->stat.st_uid = getuid();
    file->stat.st_gid = getgid();
    file->firstBlock = -1;

    file->rootDirBlock = index;
    existing_files[index] = file;
    existing_files_counter++;

    return file;
}

/// @brief Delete file
/// @param file
void RootDir::delete_file(RootFile *file) {
    int index = 0;
    while(existing_files[index] != file)
        index++;
    delete file;

    existing_files[index] = nullptr;
    existing_files_counter--;
}

/// @brief Get file from path
/// @param path
/// @return file if found, else nullptr
RootFile* RootDir::get_file(const char *path) {
    path++; // due to slash

    for(int i = 0; i < NUM_DIR_ENTRIES; i++) {
        if(existing_files[i] != nullptr && strcmp(path, existing_files[i]->name) == 0)
            return existing_files[i];
    }
    return nullptr; // if not found
}

/// @brief Get all files
/// @return all existing files
RootFile** RootDir::get_files() {
    return existing_files;
}

/// @brief Load file at given index
/// @param index
/// @return loaded file
RootFile* RootDir::load(int index) {
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, BLOCK_SIZE);
    this->blockDevice->read(ROOT_DIR_OFFSET + index, buffer);

    if(!Utils::checkContent(buffer)) {
        RootFile *file = new RootFile();
        memcpy(file, buffer, sizeof(RootFile));
        this->existing_files[index] = file;
        return file;
    }
    return nullptr;
}

/// @brief Writes changes on disk
/// @param file to be saved
bool RootDir::save_on_disk(RootFile *file) {
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, BLOCK_SIZE);
    memcpy(buffer, file, sizeof(RootFile));
    this->blockDevice->write(ROOT_DIR_OFFSET + file->rootDirBlock, buffer);
    return true;
}

/// @brief Load files after opening an EXISTING file container
void RootDir::init_root_dir() {
    for(int i = 0; i < NUM_DIR_ENTRIES; i++) {
        RootFile *file = load(i);
        existing_files[i] = file;
        if(file != nullptr)
            existing_files_counter++;
    }
}


/// @brief Initialise RootDir for an empty filesystem
void RootDir::first_init_root_dir() {
    char buffer[BLOCK_SIZE];
    for(int i = 0; i < NUM_DIR_ENTRIES; i++) {
        memset(buffer, 0, BLOCK_SIZE);
        blockDevice->write(ROOT_DIR_OFFSET + i, buffer);
    }
}