#include <iostream>
#include <sstream>

#include "Ext2SuperBlockHelper.hpp"

Ext2SuperBlockHelper::Ext2SuperBlockHelper(const std::vector<char> &ext2_dump) {
    find_super_block(ext2_dump);
}

void Ext2SuperBlockHelper::find_super_block(const std::vector<char> &ext2_dump) {
    offset = 0;

    while (true) {
        if (offset >= ext2_dump.size()) {
            std::cerr << "Superblock not found." << std::endl;
            exit(1);
        }

        auto possible_super_block = (struct ext2_super_block *) &ext2_dump.at(offset);

        if (possible_super_block->s_magic == EXT2_SUPER_MAGIC) {
            found_super_block = possible_super_block;
            std::cout << *this << std::endl;
            break;
        }

        // Skip boot records
        offset += 512;
    }
}

void Ext2SuperBlockHelper::to_stream(std::ostream &os) {
    os << "######################" << std::endl;
    os << "Superblock information" << std::endl;

    // First some general stuff about the file system
    os << "* Located at offset " << offset << " (block group " << found_super_block->s_block_group_nr << ")" << std::endl;
    os << "* File System ext2 revision " << get_ext2_revision() << std::endl;
    os << "* Volume name: " << get_volume_name() << std::endl;
    os << "* State: " << get_human_readable_state() << std::endl;

    // Now the interesting stuff
    os << "---" << std::endl;
    os << "* Block size: " << get_block_size_in_bytes() << " bytes" << std::endl;
    os << "* Total Blocks: " << get_block_count() << " (" << get_free_block_count() << " free, "
       << get_used_block_count() << " used, " << get_reserved_block_count() << " reserved for super user)" << std::endl;
    os << "* Blocks per group: " << get_blocks_per_group() << std::endl;

    os << "* Total Inodes: " << get_inode_count() << " (" << get_free_inode_count() << " free, "
       << get_used_inode_count() << " used)" << std::endl;
    os << "* Inodes per group: " << get_inodes_per_group() << std::endl;
}

std::string Ext2SuperBlockHelper::get_ext2_revision() {
    std::stringstream string_stream;
    string_stream << found_super_block->s_rev_level << "." << found_super_block->s_minor_rev_level;
    return string_stream.str();
}

std::string Ext2SuperBlockHelper::get_volume_name() {
    return found_super_block->s_volume_name;
}

std::string Ext2SuperBlockHelper::get_human_readable_state() {
    switch (found_super_block->s_state) {
        case EXT2_VALID_FS:
            return "Unmounted cleanly";
        case EXT2_ERROR_FS:
            return "Errors detected";
        default:
            return "Unknown";
    }
}

long Ext2SuperBlockHelper::get_block_size_in_bytes() {
    return 1024 << found_super_block->s_log_block_size;
}

unsigned int Ext2SuperBlockHelper::get_block_count() {
    return found_super_block->s_blocks_count;
}

unsigned int Ext2SuperBlockHelper::get_reserved_block_count() {
    return found_super_block->s_r_blocks_count;
}

unsigned int Ext2SuperBlockHelper::get_free_block_count() {
    return found_super_block->s_free_blocks_count;
}

unsigned int Ext2SuperBlockHelper::get_used_block_count() {
    return get_block_count() - get_free_block_count();
}

unsigned int Ext2SuperBlockHelper::get_blocks_per_group() {
    return found_super_block->s_blocks_per_group;
}

unsigned int Ext2SuperBlockHelper::get_inode_count() {
    return found_super_block->s_inodes_count;
}

unsigned int Ext2SuperBlockHelper::get_free_inode_count() {
    return found_super_block->s_free_inodes_count;
}

unsigned int Ext2SuperBlockHelper::get_used_inode_count() {
    return get_inode_count() - get_free_inode_count();
}

unsigned int Ext2SuperBlockHelper::get_inodes_per_group() {
    return found_super_block->s_inodes_per_group;
}
