#include <iostream>
#include <fstream>

struct Data {
    uint32_t data1;
    uint32_t data2;
};

bool verify_asc_order(std::string &filename)
{
    size_t count = 0;
    std::ifstream in_file(filename.c_str(), std::ios::in | std::ios::binary );
    if (in_file.fail()) {
      std::cerr << "debug: unable to open input file for reading" << std::endl;
      return false;
    }
    Data data;
    while (in_file.read(reinterpret_cast<char *>(&data), sizeof(data))) {
        if (data.data1 > data.data2) {
            return false;
        }
    }
    in_file.close();
    return true;
}

int main(int argc, char** argv)
{   
    if (argc != 2) {
        std::cerr << "not enough arguments" << argc << std::endl;
        return 1;
    }
    std::string file_to_dump = argv[1];
    bool is_asc = verify_asc_order(file_to_dump);
    if (!is_asc) {
        std::cerr << "file is not sorted in ASC order" << std::endl;
    } else {
        std::cout << "file is sorted in ASC order" << std::endl;
    }
    return is_asc;
}