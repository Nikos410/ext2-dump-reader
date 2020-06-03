#include "DumpReader.hpp"

DumpReader::DumpReader(std::string &dump_file_path) {
    // Open the file as binary and seek to the end
    dump_file_stream = std::ifstream(dump_file_path, std::ios::binary | std::ios::ate);

    // Since we seeked to the end immediately, our current read position is the fileSize
    dump_file_size = dump_file_stream.tellg();
}

std::vector<char>& DumpReader::read_into_buffer(long limit) {
    buffer_size = limit > 0 && dump_file_size > limit ? limit : dump_file_size;

    // Seek back to the beginning
    dump_file_stream.seekg(0, std::ios::beg);

    // Create buffer with the size we detected earlier
    buffer = std::vector<char>(buffer_size);

    // Read to the buffer
    if (dump_file_stream.read(buffer.data(), buffer_size)) {
        return buffer;
    }

    std::cerr << "Could not read file contents into buffer." << std::endl;
    exit(1);
}
