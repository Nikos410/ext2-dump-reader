#include <iostream>
#include <sstream>

#include "BlockHelper.hpp"

BlockHelper::BlockHelper(char* ext2_dump) {
    ext2_dump_ = ext2_dump;
    ext2_super_block* primary_super_block = as_superblock_if_present(ext2_dump_ + SUPER_BLOCK_OFFSET);

    if (primary_super_block == nullptr) {
        std::cerr << "Primary super Block not found at offset " << SUPER_BLOCK_OFFSET << std::endl;
        exit(1);
    } else {
        super_block_ = primary_super_block;
    }
}

ext2_super_block* BlockHelper::as_superblock_if_present(char *ptr) {
    ext2_super_block* possible_super_block = (ext2_super_block *) ptr;

    if (possible_super_block->s_magic == EXT2_SUPER_MAGIC) {
        return possible_super_block;
    } else {
        return nullptr;
    }
}

void BlockHelper::print_superblock_information() {
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

ext2_super_block* BlockHelper::get_super_block() {
    return super_block_;
}

char *BlockHelper::get_block(unsigned int block_number) {
    if (block_number > super_block_->s_blocks_count) {
        std::cerr << "Block number exceeds block count." << std::endl;
        exit(1);
    }

    return ext2_dump_ + (block_number * get_block_size_in_bytes());
}

char *BlockHelper::get_block_after(const char *after) {
    long offset = after - ext2_dump_;
    unsigned int current_block = offset / get_block_size_in_bytes();
    return get_block(current_block + 1);
}

std::string BlockHelper::get_ext2_revision() {
    std::stringstream string_stream;
    string_stream << super_block_->s_rev_level << "." << super_block_->s_minor_rev_level;
    return string_stream.str();
}

std::string BlockHelper::get_human_readable_state() {
    switch (super_block_->s_state) {
        case EXT2_VALID_FS:
            return "Unmounted cleanly";
        case EXT2_ERROR_FS:
            return "Errors detected";
        default:
            return "Unknown";
    }
}

long BlockHelper::get_block_size_in_bytes() {
    return 1024 << super_block_->s_log_block_size;
}

unsigned int BlockHelper::get_used_block_count() {
    return super_block_->s_blocks_count - super_block_->s_free_blocks_count;
}

unsigned int BlockHelper::get_used_inode_count() {
    return super_block_->s_inodes_count - super_block_->s_free_inodes_count;
}
