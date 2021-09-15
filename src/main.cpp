#include <stdio.h>

#include <fstream>
#include <stdlib.h>
#include <string.h>
#include "loader.hpp"
#include "x86/loader_64.hpp"
#include "x86/emulator_64.hpp"
char* rd_file(const char* path)
{
    std::ifstream stream;
    stream.open(path, std::ios::in | std::ios::binary);
    if(!stream.is_open())
    {
        return nullptr;
    }
    
    char* data;
    stream.seekg(0, std::ios::end);

    size_t file_size = stream.tellg();

    data = new char[file_size+1];

    memset(data, 0, file_size+1);

    stream.seekg(0, std::ios::beg);

    stream.read(data, file_size);

    stream.close();
    return data;
}
int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("usage: fprof [file_to_execute] \n");
        return 1;
    }



    for(int i = 1; i < argc; i++)
    {
        char* d = rd_file(argv[i]);
        fp::memory_map* map = new fp::memory_map;
        fp::loader* l = fp::loader::load_data<fp::x86_64_loader>(map, d);
        if(l == NULL)
        {
            printf("fuck gcc \n");
        }
        (void)l;

        fp::emulator* em = fp::emulator::load_emulator<fp::x86_emulator>(map, l);

        em->start();
        if(d == NULL)
        {
            printf("can't open file %s \n", argv[1]);
            return 1;
        }
        printf("%i = %s \n", i, argv[i]);

        delete[] d;
    }
    printf("hello world \n");
    return 0;
}
