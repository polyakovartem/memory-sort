#include <string>
#include <memory>

class BinaryFileIterator {
public:
    BinaryFileIterator(const std::string& file_name);
    BinaryFileIterator(const BinaryFileIterator& copy);
    BinaryFileIterator(BinaryFileIterator&& copy);
    ~BinaryFileIterator();

    bool next();
    bool next(uint32_t& data);
    const uint32_t& top();

private:
    uint32_t current;
    std::string filename;
    std::shared_ptr<std::ifstream> file_stream;
};