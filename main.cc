#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "please specify source and destination files" << std::endl;
        std::cout << "Usage: memorysort file_to_sort sorted_file" << std::endl;
    }
    return 0;
}
