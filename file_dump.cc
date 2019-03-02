#include <iostream>
#include <fstream>

const size_t OUTPUT_WIDTH = 10;

void debug_dump_file(std::string &filename)
{
    size_t count = 0;
    std::ifstream in_file(filename.c_str(), std::ios::in | std::ios::binary );
    if (in_file.fail()) {
      std::cerr << "debug: unable to open input file for reading" << std::endl;
      return;
    }
    uint32_t data;
    while (in_file.read(reinterpret_cast<char *>(&data), sizeof(data))) {
        std::cout << data << " ";
        if (count++ > OUTPUT_WIDTH) {
            std::cout << std::endl;
            count = 0;
        }
    }
    in_file.close();
}

int main(int argc, char** argv)
{   
    if (argc != 2) {
        return 1;
    }
    std::string file_to_dump = argv[1];
    debug_dump_file(file_to_dump);
    return 0;
}