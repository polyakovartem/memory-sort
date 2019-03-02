#include <fstream>

#include "bfiterator.h"


BinaryFileIterator::BinaryFileIterator(const std::string &file_name): filename(file_name)
{
    file_stream = std::make_shared<std::ifstream>(filename.c_str(), std::ios::in | std::ios::binary);
}

BinaryFileIterator::BinaryFileIterator(const BinaryFileIterator &copy)
{
    file_stream = copy.file_stream;
    filename = copy.filename;
}

BinaryFileIterator::BinaryFileIterator(BinaryFileIterator &&copy)
    :file_stream(std::move(copy.file_stream)),
    filename(std::move(copy.filename)),
    current(std::exchange(copy.current, 0))
{
}

BinaryFileIterator::~BinaryFileIterator()
{
    if (file_stream) {
        file_stream->close();
    }
}

bool BinaryFileIterator::next(uint32_t& data)
{
    if (file_stream->fail()) {
        return false;
    }

    file_stream->read(reinterpret_cast<char *>(&data), sizeof(data));
    current = data;
    return !file_stream->eof();
}

const uint32_t& BinaryFileIterator::top()
{
    return current;
}