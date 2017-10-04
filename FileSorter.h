#pragma once

#include <vector>
#include <algorithm>
#include "helper/FileHelper.h"
#include "Segment.h"
#include "buffer/FileReadBuffer.h"
#include "buffer/FileWriteBuffer.h"

template<typename Type>

class FileSorter
{
public:
    explicit FileSorter(std::string path_to_file, size_t buffer_size)
        : path_to_file(path_to_file)
    {
        this->buffer_size = static_cast<size_t>(buffer_size / size_type) * size_type;
    }

    void sortBinaryFile()
    {
        std::fstream file1(path_to_file, std::ios::binary | std::ios::in | std::ios::out);
        size_t file_size = FileHelper::getFileSize(file1);
        file1.seekg(0, std::ios::beg);

        int count_equal_segments = file_size / buffer_size;
        std::vector<Type> buffer(buffer_size / size_type);
        std::vector<Segment> file_segments;
        int i;
        for(i = 0; i < count_equal_segments; ++i)
        {
            sortPartInPlace(file1, buffer, buffer_size);
            file_segments.push_back({i * buffer_size, (i + 1) * buffer_size});
        }

        size_t remainder = file_size % buffer_size;
        if(remainder)
        {
            sortPartInPlace(file1, buffer, remainder);
            file_segments.push_back({i * buffer_size, i * buffer_size + remainder});
        }

        if(file_size <= buffer_size) { return; }
        buffer = std::vector<Type>();

        std::string path_to_temp_file = "~temp.bin";
        std::string merge_file = path_to_temp_file;
        std::string path_to_sorted_file = path_to_file;
        std::fstream(path_to_temp_file, std::ios::out);
        std::fstream file2(path_to_temp_file, std::ios::binary | std::ios::in | std::ios::out);

        while(file_segments.size() > 1)
        {
            std::vector<Segment> new_segments;
            for(int i = 0; i < file_segments.size(); i += 2)
            {
                Segment left_segment = file_segments[i];
                Segment right_segment = file_segments[i + 1];
                if(file_segments.size() == i + 1)
                {
                    right_segment = {left_segment.end, left_segment.end};
                }
                mergeToFile(file1, file2, left_segment, right_segment, buffer_size);
                new_segments.push_back({left_segment.beg, right_segment.end});
            }
            std::swap(file1, file2);
            std::swap(merge_file, path_to_sorted_file);
            file_segments = new_segments;
        }

        if(path_to_sorted_file == path_to_file)
        {
            remove(path_to_temp_file.data());
        }
        else
        {
            rename(path_to_sorted_file.data(), path_to_file.data());
        }
    }

private:
    FileSorter() {}

    void sortPartInPlace(std::fstream& file, std::vector<Type>& buffer, size_t streamsize)
    {
        streamsize = static_cast<size_t>(streamsize / size_type) * size_type;
        if(!streamsize) { return; }
        file.read(reinterpret_cast<char*>(&buffer[0]), streamsize);
        std::sort(buffer.begin(), buffer.begin() + (streamsize / size_type));
        file.seekg(-file.gcount(), std::ios::cur);
        file.write(reinterpret_cast<char*>(&buffer[0]), streamsize);
    }

    void mergeToFile(std::fstream& in, std::fstream& out, Segment s1, Segment s2, size_t merge_buffer)
    {
        merge_buffer = (merge_buffer / 2 / size_type) * size_type;
        FileReadBuffer<Type> left_f(in, s1, merge_buffer / 2);
        FileReadBuffer<Type> right_f(in, s2, merge_buffer / 2);
        FileWriteBuffer<Type> ofile(out, {s1.beg, s2.end}, merge_buffer);

        size_t left_i = 0, right_i = 0;

        while(left_f.hasValue(left_i) && right_f.hasValue(right_i))
        {
            ofile.write(left_f[left_i] <= right_f[right_i] ? left_f[left_i++] : right_f[right_i++]);
        }
        while(left_f.hasValue(left_i))
        {
            ofile.write(left_f[left_i++]);
        }
        while(right_f.hasValue(right_i))
        {
            ofile.write(right_f[right_i++]);
        }
        ofile.flush();
    }

    std::string path_to_file;
    size_t buffer_size;
    int size_type = sizeof(Type);
};