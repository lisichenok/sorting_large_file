#pragma once
#include <random>
#include <fstream>

namespace FileSorterHelper {
template<typename Type>
bool checkBinaryFile(std::string path_to_file)
{
    int size_type = sizeof(Type);
    Type value, min = std::numeric_limits<Type>::min();
    std::ifstream file(path_to_file, std::ios::binary);

    while(file.good())
    {
        file.read(reinterpret_cast<char*>(&value), size_type);
        if(value < min)
        {
            return false;
        }
        min = value;
    }
    file.close();
    return true;
}
}