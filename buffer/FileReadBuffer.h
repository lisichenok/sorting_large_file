#pragma once
#include "FileBufferBase.h"

template<typename Type>
class FileReadBuffer : public FileBufferBase<Type>
{

public:
    FileReadBuffer(std::fstream& file, Segment file_segment, size_t buffer_size = sizeof(Type))
        : FileBufferBase<Type>(file_segment, buffer_size)
        , file(file)
    {}

    bool hasValue(size_t i)
    {
        i *= this->size_type;
        return this->segment.size() && i >= 0 && i <= this->segment.size() - this->size_type;
    }

    Type& operator[](size_t i)
    {
        if(i >= this->cur_pos + this->buffer.size() || i < this->cur_pos || !this->is_init)
        {
            this->cur_pos = i;
            size_t seekp = this->segment.beg + (this->cur_pos * this->size_type);
            file.seekp(seekp);
            size_t read_size = this->segment.end - seekp >= this->buffer_size ? this->buffer_size : this->segment.end - seekp;
            file.read(reinterpret_cast<char*>(&this->buffer[0]), read_size);
            this->is_init = true;
        }
        return this->buffer[i % this->buffer.size()];
    }

private:
    bool is_init = false;
    std::fstream& file;
};