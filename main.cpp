#include <glob.h>
#include <cstdio>
#include <fstream>
#include "helper/FileHelper.h"
#include "FileSorter.h"
#include "helper/FileSorterHelper.h"

int main(int argc, char* argv[])
{
    std::string input_file = "input.bin";

    size_t file_size = 1 * static_cast<size_t>(1024 * 1024 * 1024); // 1 Гбайт;
    size_t buffer_size = 128 * static_cast<size_t>(1024 * 1024);    // 128 Мбайт;

    printf("Размер буфера: %f Мбайт\n", static_cast<float>(buffer_size / 1024 / 1024));
    if(!std::ifstream(input_file))
    {
        float size_gb = static_cast<long double>(file_size) / 1024 / 1024 / 1024;
        printf("Создаю файл '%s' %f Гбайт ... ", input_file.data(), size_gb);
        fflush(stdout);
        long int create_time = time(0);
        FileHelper::createRandomBinaryFile<int64_t>(input_file, file_size);
        printf("OK");
        printf(" (%li сек.)\n", time(0) - create_time);
    }

    printf("Сортирую '%s' ... ", input_file.data());
    fflush(stdout);
    long int sort_time = time(0);
    FileSorter<int64_t> file(input_file, buffer_size);
    file.sortBinaryFile();
    printf("OK");
    printf(" (%li сек.)\n", time(0) - sort_time);

    printf("Проверяю последовательность значений ... ");
    fflush(stdout);
    bool ok = FileSorterHelper::checkBinaryFile<int64_t>(input_file);
    printf(ok ? "(все верно)\n" : "(неверно)\n");

//    FileHelper::traceBinaryFile<int64_t>("input.bin");

    return 0;
}

