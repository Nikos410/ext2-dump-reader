#include <iostream>
#include <vector>

#include "Ext2DumpReader.hpp"
#include "Ext2BlockHelper.hpp"
#include "InodeHelper.hpp"

inline std::ostream& operator <<(std::ostream& os, std::vector<char>& vector) {
    for (char& current : vector) {
        os << current;
    }

    return os;
}

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
    block_helper.print_superblock_information();

    ext2_super_block* super_block = block_helper.get_super_block();
    ext2_group_desc* first_group_descriptor = (ext2_group_desc*) block_helper.get_block_after((char*)super_block);
    bool* block_bitmap = (bool*) block_helper.get_block(first_group_descriptor->bg_block_bitmap);
    bool* inode_bitmap = (bool*) block_helper.get_block(first_group_descriptor->bg_inode_bitmap);
    ext2_inode* inode = (ext2_inode*) block_helper.get_block(first_group_descriptor->bg_inode_table);

    for (int i = 0; i < super_block->s_inodes_per_group; i++) {
        InodeHelper inode_helper(block_helper, inode);
        if (inode_helper.is_directory()) {
            std::cout << "Inode " << i << ": Directory." << std::endl;
            std::cout << inode_helper.copy_data() << std::endl;
        }

        if (inode_helper.is_regularFile()) {
            std::cout << "Inode " << i << ": Regular file." << std::endl;
            std::cout << inode_helper.copy_data() << std::endl;
        }

        inode++;
    }

    return EXIT_SUCCESS;
}
