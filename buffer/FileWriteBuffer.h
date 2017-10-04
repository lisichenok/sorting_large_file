#pragma once
#include "FileBufferBase.h"

template<typename Type>
class FileWriteBuffer : public FileBufferBase<Type>
{

public:
    FileWriteBuffer(std::fstream& file, Segment file_segment, size_t buffer_size = sizeof(Type))
        : FileBufferBase<Type>(file_segment
        , buffer_size), file(file)
    {
        file.seekp(file_segment.beg);
    }

    void write(Type& value)
    {
        this->buffer[count_values_in_buffer++] = value;
        if(count_values_in_buffer == this->buffer.size())
        {
            file.write(reinterpret_cast<char*>(&this->buffer[0]), count_values_in_buffer * this->size_type);
            count_values_in_buffer = 0;
        }
    }

    void flush()
    {
        file.write(reinterpret_cast<char*>(&this->buffer[0]), count_values_in_buffer * this->size_type);
    }

    int count_values_in_buffer = 0;
    std::fstream& file;
};