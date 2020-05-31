#ifndef EXT2SUPERBLOCKHELPER_HPP
#define EXT2SUPERBLOCKHELPER_HPP

#include <vector>
#include <linux/fs.h>
#include <ext2fs/ext2_fs.h>

class Ext2BlockHelper {
public:
    Ext2BlockHelper(char* ext2_dump);

    void print_superblock_information();

    ext2_super_block* get_super_block();
    char* get_block(unsigned int block_number);
    char* get_block_after(const char* after);

    long get_block_size_in_bytes();
private:
    std::string get_ext2_revision();
    std::string get_human_readable_state();
    unsigned int get_used_block_count();
    unsigned int get_used_inode_count();

    const static unsigned int SUPER_BLOCK_OFFSET = 1024;
    char* ext2_dump_;
    ext2_super_block* super_block_ = nullptr;
};

#endif // EXT2SUPERBLOCKHELPER_HPP
