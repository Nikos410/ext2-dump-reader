#include <iostream>
#include <vector>

#include "Ext2DumpReader.hpp"
#include "Ext2BlockHelper.hpp"

int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Ambiguous parameters. Please provide only the path of the ext2 dump file." << std::endl;
        exit(1);
    }
    std::string dump_file_path = argv[1];

    std::cout << "Loading ext2 dump from " << dump_file_path <<std::endl;

    Ext2DumpReader dump_reader(dump_file_path);
    std::vector<char>& ext2_dump = dump_reader.read_into_buffer(NO_LIMIT);

    std::cout << "Successfully read ext2 dump with size " << ext2_dump.size() << " into memory." << std::endl;
    Ext2BlockHelper block_helper(ext2_dump.data());

    return EXIT_SUCCESS;
}
