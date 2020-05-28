#ifndef DUMPREADER_HPP
#define DUMPREADER_HPP

#include <iostream>
#include <vector>
#include <fstream>

const static char NO_LIMIT = -1;

class Ext2DumpReader {
public:
    explicit Ext2DumpReader(std::string& dump_file_path);
    std::vector<char>& read_into_buffer(long limit);
private:
    std::ifstream dump_file_stream;
    long dump_file_size;
    long buffer_size;
    std::vector<char> buffer;
};


#endif
