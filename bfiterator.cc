#include <fstream>

#include "bfiterator.h"


BinaryFileIterator::BinaryFileIterator(const std::string &file_name): filename(file_name)
{
    file_stream = std::make_shared<std::ifstream>(filename.c_str(), std::ios::in | std::ios::binary);
    next();
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

bool BinaryFileIterator::next()
{
    if (file_stream->fail()) {
        return false;
    }

    // TODO add chache reading
    file_stream->read(reinterpret_cast<char *>(&current), sizeof(current));
    return !file_stream->eof();
}

bool BinaryFileIterator::next(uint32_t& data)
{
    bool is_success = next();
    data = current;

    return is_success;
}

const uint32_t& BinaryFileIterator::top()
{
    return current;
}
