#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include "config.hpp"
#include "utils/log.hpp"
namespace fp
{

struct memory_map_entry
{
    uintptr_t start;
    uintptr_t end;
    void *data;

    memory_map_entry(){};
    memory_map_entry(uintptr_t _start, uintptr_t _end, void *_data)
    {
        start = _start;
        end = _end;
        data = _data;
    }
};

class memory_map
{
    std::vector<memory_map_entry> _entry;

    memory_map_entry cached;
    const memory_map_entry *get_memory_map(uintptr_t addr);

public:
    void *get_data_at_addr(uintptr_t addr, size_t size);

    template <typename T>
    void write(uintptr_t addr, const T &value)
    {
        if (addr % sizeof(T) != 0)
        {
            error$("writing with unaligned addr");
        }
        *(T *)get_data_at_addr(addr, sizeof(T)) = value;
    }

    template <typename T>
    T read(uintptr_t addr)
    {
        if (addr % sizeof(T) != 0)
        {
            error$("writing with unaligned addr");
        }

        return *(T *)get_data_at_addr(addr, sizeof(T));
    }

    void map_memory(uintptr_t virt_addr, size_t size, const void *data);
};
} // namespace fp
