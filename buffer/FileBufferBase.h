#pragma once
#include <fstream>
#include <vector>
#include "../Segment.h"

template<typename Type>
class FileBufferBase
{
public:

    explicit FileBufferBase(Segment file_segment, size_t buffer_size)
        : segment(file_segment)
        , buffer_size(buffer_size)
    {}

    int size_type = sizeof(Type);
    size_t cur_pos = 0;
    size_t buffer_size = size_type;
    std::vector<Type> buffer = std::vector<Type>(buffer_size / size_type);
    Segment segment;
};