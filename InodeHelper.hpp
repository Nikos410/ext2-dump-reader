#ifndef INODEHELPER_HPP
#define INODEHELPER_HPP

#include <string>
#include "Ext2BlockHelper.hpp"

class InodeHelper {
public:
    InodeHelper(Ext2BlockHelper& block_helper, ext2_inode* inode): block_helper_{block_helper}, inode_{inode} {
        data_ = std::vector<char>(inode->i_size);
    };
    bool is_directory();
    bool is_regularFile();

    std::vector<char>& copy_data();

private:
    void copy_data_block_if_neccessary(unsigned int block_number);
    void copy_indirect_block_if_neccessary(unsigned int block_number);
    void copy_doubly_indirect_block_if_neccessary(unsigned int block_number);
    void copy_trebly_indirect_block_if_neccessary(unsigned int block_number);

    Ext2BlockHelper& block_helper_;
    ext2_inode* inode_;
    std::vector<char> data_;
};


#endif
