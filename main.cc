//#include <algorithm>
#include <future>
#include <fstream>
#include <iostream>
//#include <iterator>
//#include <list>
#include <sys/stat.h>
#include <memory>
//#include <thread>
#include <vector>

#include "bfiterator.h"

/***
 * read file by chunks
 * sort each chunk 
 * write each sorted chunk into temporary file
 * use "kind of" merge sort to create new sorted file
*/

const size_t MAX_MEMORY = 256*1024*1024; // 256MB
const size_t BUFFER_SZ = 100*1024*1024; // 100MB
const size_t CHUNK_SZ = 10*1024*1024; // 50MB
const size_t WORKERS_COUNT = MAX_MEMORY / CHUNK_SZ;
const size_t CHUNK_COUNT = CHUNK_SZ/sizeof(uint32_t);
const size_t MULT = 4;
const char* TMP_DIR = "tmp/";

bool verify_arguments(int argc, char** argv);
bool verify_filesize(const std::string &filename);
bool split_file_into_sorted_chunks(const std::string &filename);
bool merge_sorted_files(const std::string &dst_file);

int main(int argc, char** argv)
{
    if (!verify_arguments(argc, argv)) {
        return 1;
    }

    const std::string src_file = argv[1];
    if (!verify_filesize(src_file)) {
        return 1;
    }

    // there is no point to try splitting file in multiple thread,
    // as sequencial file access is in general faster that random access
    if (!split_file_into_sorted_chunks(src_file)) {
        return 1;
    }

    const std::string dst_file = argv[2];
    if (!merge_sorted_files(dst_file)) {
        return 1;
    }

    return 0;
}

bool merge_sorted_files(const std::string &dst_file)
{
    size_t file_idx = 0;
    std::vector <std::shared_ptr<BinaryFileIterator> > tmp_files;
    
    // open all tmp files
    while (true) {
        struct stat results;
        std::string tmp_file_path = TMP_DIR+std::to_string(file_idx++);
        if (stat(tmp_file_path.c_str(), &results) != 0) {
            break;
        }
        tmp_files.push_back(std::make_shared<BinaryFileIterator>(tmp_file_path));
    }

    // merge files
    std::ofstream out_file(dst_file, std::ios::out | std::ios::binary);
    for (auto it = tmp_files.begin(); it != tmp_files.end(); it++) {
        uint32_t data;
        while ((*it)->next(data)) {
            out_file.write(reinterpret_cast<char *>(&data), sizeof(data));
        }
    }
    out_file.close();
    return true;
}

bool dump_chunk_to_tmp_file(std::vector<uint32_t> &buffer)
{
    static size_t file_idx = 0;

    // dump buffer to temporary file
    std::string tmp_file_path = TMP_DIR+std::to_string(file_idx++);
    std::ofstream out_file(tmp_file_path, std::ios::out | std::ios::binary);
    if (out_file.fail()) {
        return true;
    }

    for (auto it = buffer.begin(); it != buffer.end(); it++) {
        uint32_t data = *it;
        out_file.write(reinterpret_cast<char *>(&data), sizeof(data));
    }
    out_file.close();
    return true;
}

void sort_chunk(std::vector<uint32_t> &&buffer)
{
    // sort the buffer
    std::sort(buffer.begin(), buffer.begin()+buffer.size());
    dump_chunk_to_tmp_file(buffer);
}

bool get_file_chunk(std::ifstream &in_file, std::vector<uint32_t> &buffer,
                    size_t chunk_count=CHUNK_COUNT)
{
    if (in_file.eof()) {
        return false;
    }

    uint32_t data;
    while (buffer.size() <= chunk_count &&
           in_file.read(reinterpret_cast<char *>(&data), sizeof(data))) {
        buffer.push_back(data);
    }

    return true;
}

bool split_file_into_sorted_chunks(const std::string &filename)
{
    std::ifstream in_file(filename, std::ios::in | std::ios::binary );
    if (in_file.fail()) {
      std::cerr << "unable to open file input file for reading" << std::endl;
      return false;
    }
    // copy all data into buffer
    while (true){
        std::vector<uint32_t> buffer;
        if (!get_file_chunk(in_file, buffer)) {
            break;
        }
        std::async(std::launch::async, sort_chunk, buffer);        
    }
    in_file.close();

    return true;
}

bool verify_arguments(int argc, char** argv)
{
    argv = argv;
    if (argc != 3)
    {
        std::cerr << "please specify source and destination files" << std::endl << std::endl;
        std::cerr << "Usage: memorysort file_to_sort sorted_file" << std::endl;
        return false;
    }
    return true;
}

bool verify_filesize(const std::string &filename)
{
    struct stat results;

    if (stat(filename.c_str(), &results) == 0) {
        if (results.st_size % MULT != 0) {
            std::cerr << "file size cannot be devided by " << MULT << std::endl;
        }
        return true;
    }
    return false;
}
// TODO unit test 
