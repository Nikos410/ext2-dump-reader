#include <iostream>
#include <fstream>
#include <vector>
#include <linux/fs.h>
#include <ext2fs/ext2_fs.h>

const int NO_LIMIT = -1;

std::vector<char> read_dump(std::ifstream& openFile, int bufferSize) {
    // Seek back to the beginning
    openFile.seekg(0, std::ios::beg);

    // Create buffer with the sice we detected earlier
    std::vector<char> buffer(bufferSize);

    // Read to the buffer
    if (openFile.read(buffer.data(), bufferSize)) {
        return buffer;
    }

    std::cerr << "Could not read file contents into buffer." << std::endl;
    exit(1);
}

int getBufferSize(unsigned int fileSize, int limit) {
    if (limit > 0 && fileSize > limit) {
        return limit;
    } else {
        return fileSize;
    }
}

std::vector<char> read_dump(std::string& filePath, int limit) {
    // Open the file as binary and seek to the end
    std::ifstream openFile(filePath, std::ios::binary | std::ios::ate);

    // Since we seeked to the end immediately, our current read position is the fileSize
    std::streamsize fileSize = openFile.tellg();

    return read_dump(openFile, getBufferSize(fileSize, limit));
}

std::vector<char> read_dump(std::string filePath) {
    return read_dump(filePath, NO_LIMIT);
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Ambiguous parameters. Please provide only the path of the ext2 dump file." << std::endl;
        exit(1);
    }

    std::vector<char> ext2Dump = read_dump(argv[1]);
    std::cout << "Successfully read ext2 dump with size " << ext2Dump.size() << " into memory." << std::endl;

    int offset = 0;

    // Search for the superblock
    while (true) {
        ext2_super_block* superBlock = (struct ext2_super_block*) &ext2Dump.at(offset);

        if(superBlock->s_magic == EXT2_SUPER_MAGIC) {
            std::cout << "Superblock found at offset " << offset << std::endl;
            break;
        } else {
            offset += 512;
        }
    }

    return EXIT_SUCCESS;
}