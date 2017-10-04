
#pragma once

#include <cstdio>
#include <fstream>
#include <iostream>
#include "NumberHelper.h"

namespace FileHelper {
template<typename Type>
void createRandomBinaryFile(std::string file, size_t size)
{
    int size_type = sizeof(Type);
    std::ofstream out(file, std::ios_base::binary);
    size_t count_values = size / size_type;
    while(count_values--)
    {
        size_t value = NumberHelper::getRandomNum<Type>();
        out.write(reinterpret_cast<char*>(&value), size_type);
    }
    out.close();
}

template<typename Type>
void traceBinaryFile(std::string path_to_file)
{
    std::cout << "trace file: " << path_to_file << std::endl;
    int size_type = sizeof(Type);
    Type value;
    std::ifstream file(path_to_file, std::ios::binary);
    size_t i = 0;
    while(file.good())
    {
        file.read(reinterpret_cast<char*>(&value), size_type);
        if(file)
        {
            std::cout << value << std::endl;
        }
    }
    file.close();
}

auto getFileSize(std::fstream& file)
{
    size_t cur_pos = file.tellp();
    file.seekp(0, std::ios::end);
    size_t file_size = file.tellp();
    file.seekp(cur_pos);
    return file_size;
}
}
