#include <iostream>
#include <vector>
#include <sstream>

#include "DumpReader.hpp"
#include "BlockHelper.hpp"
#include "InodeHelper.hpp"

inline std::ostream& operator <<(std::ostream& os, std::vector<char>& vector) {
    for (char& current : vector) {
        os << current;
    }

    return os;
}

void read_block_group(ext2_group_desc* group_descriptor, BlockHelper& block_helper) {
    ext2_super_block* super_block = block_helper.get_super_block();
    ext2_inode* inode = (ext2_inode*) block_helper.get_block(group_descriptor->bg_inode_table);

    for (int i = 0; i < super_block->s_inodes_per_group; i++) {
        InodeHelper inode_helper(block_helper, inode);
        if (inode_helper.is_directory()) {
            std::cout << "Inode " << i << ": Directory." << std::endl;
            std::cout << inode_helper.copy_data() << std::endl;
        }

        if (inode_helper.is_regularFile()) {
            std::cout << "Inode " << i << ": Regular file." << std::endl;
            std::vector<char>& data = inode_helper.copy_data();

            std::ofstream inode_file;
            std::stringstream file_name;
            file_name << "inode_";
            file_name << i;
            inode_file.open(file_name.str().c_str());
            inode_file << data;
            inode_file.close();

            data.clear();
        }

        inode++;
    }
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Ambiguous parameters. Please provide only the path of the ext2 dump file." << std::endl;
        exit(1);
    }
    std::string dump_file_path = argv[1];

    std::cout << "Loading ext2 dump from " << dump_file_path <<std::endl;

    DumpReader dump_reader(dump_file_path);
    std::vector<char>& ext2_dump = dump_reader.read_into_buffer(NO_LIMIT);

    std::cout << "Successfully read ext2 dump with size " << ext2_dump.size() << " into memory." << std::endl;
    BlockHelper block_helper(ext2_dump.data());
    block_helper.print_superblock_information();

    ext2_super_block* super_block = block_helper.get_super_block();
    ext2_group_desc* group_descriptor = (ext2_group_desc*) block_helper.get_block_after((char*)super_block);

    // TODO: Do this better
    unsigned int group_count = super_block->s_blocks_count / super_block->s_blocks_per_group;
    if (super_block->s_blocks_count % super_block->s_blocks_per_group != 0) {
        group_count++;
    }

    std::cout << "Block group count: " << group_count << std::endl;

    for (int i = 0; i < group_count; i++) {
        read_block_group(group_descriptor, block_helper);
        group_descriptor++;
    }

    ext2_dump.clear();

    return EXIT_SUCCESS;
}
