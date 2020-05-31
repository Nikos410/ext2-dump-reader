#include <iostream>
#include <sstream>

#include "Ext2BlockHelper.hpp"

Ext2BlockHelper::Ext2BlockHelper(char* ext2_dump) {
    ext2_dump_ = ext2_dump;
    super_block_ = (ext2_super_block *) (ext2_dump + SUPER_BLOCK_OFFSET);

    if (super_block_->s_magic != EXT2_SUPER_MAGIC) {
        std::cerr << "Super Block not found at offset " << SUPER_BLOCK_OFFSET << std::endl;
        exit(1);
    }
}

void Ext2BlockHelper::print_superblock_information() {
    std::cout << "######################" << std::endl;
    std::cout << "Superblock information" << std::endl;

    // First some general stuff about the file system
    std::cout << "* Located in block group " << super_block_->s_block_group_nr << std::endl;
    std::cout << "* Ext2 revision: " << get_ext2_revision() << std::endl;
    std::cout << "* Volume name: " << super_block_->s_volume_name << std::endl;
    std::cout << "* State: " << get_human_readable_state() << std::endl;

    // Now the interesting stuff ^^
    std::cout << "* Total Blocks: " << super_block_->s_blocks_count << " (" << super_block_->s_free_blocks_count << " free, "
       << get_used_block_count() << " used, " << super_block_->s_r_blocks_count << " reserved for super user)" << std::endl;
    std::cout << "* Block size: " << super_block_->s_log_block_size << " (" << get_block_size_in_bytes() << " bytes)" << std::endl;
    std::cout << "* Blocks per group: " << super_block_->s_blocks_per_group << std::endl;

    std::cout << "* Total Inodes: " << super_block_->s_inodes_count << " (" << super_block_->s_free_inodes_count << " free, "
       << get_used_inode_count() << " used)" << std::endl;
    std::cout << "* Inodes per group: " << super_block_->s_inodes_per_group << std::endl;
    std::cout << "######################" << std::endl;
}

ext2_super_block* Ext2BlockHelper::get_super_block() {
    return super_block_;
}

char *Ext2BlockHelper::get_block(unsigned int block_number) {
    if (block_number > super_block_->s_blocks_count) {
        std::cerr << "Block number exceeds block count." << std::endl;
        exit(1);
    }

    return ext2_dump_ + (block_number * get_block_size_in_bytes());
}

char *Ext2BlockHelper::get_block_after(const char *after) {
    long offset = after - ext2_dump_;
    unsigned int current_block = offset / get_block_size_in_bytes();
    return get_block(current_block + 1);
}

std::string Ext2BlockHelper::get_ext2_revision() {
    std::stringstream string_stream;
    string_stream << super_block_->s_rev_level << "." << super_block_->s_minor_rev_level;
    return string_stream.str();
}

std::string Ext2BlockHelper::get_human_readable_state() {
    switch (super_block_->s_state) {
        case EXT2_VALID_FS:
            return "Unmounted cleanly";
        case EXT2_ERROR_FS:
            return "Errors detected";
        default:
            return "Unknown";
    }
}

long Ext2BlockHelper::get_block_size_in_bytes() {
    return 1024 << super_block_->s_log_block_size;
}

unsigned int Ext2BlockHelper::get_used_block_count() {
    return super_block_->s_blocks_count - super_block_->s_free_blocks_count;
}

unsigned int Ext2BlockHelper::get_used_inode_count() {
    return super_block_->s_inodes_count - super_block_->s_free_inodes_count;
}
