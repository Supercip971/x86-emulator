#include "memory.hpp"
#include <string.h>
#include "utils/log.hpp"
namespace fp 
{
    void memory_map::map_memory(uintptr_t virt_addr, size_t size, const void* data)
    {
        log$("mapping: {}-{}", virt_addr, virt_addr+size);
        void* data_copy = malloc(size);
        memcpy(data_copy, data, size);

        _entry.push_back(memory_map_entry(virt_addr, virt_addr+size, data_copy)); 
    }

    const memory_map_entry* memory_map::get_memory_map(uintptr_t addr)
    {
        if(cached.start != 0 && cached.start <= addr && cached.end >= addr )
        {
            return &cached;
        }

        for(auto entry : _entry)
        {
            if(entry.start <= addr && entry.end >= addr)
            {
                cached = entry;
                return &cached; 
            }
        }
        return nullptr;
    }


    void* memory_map::get_data_at_addr(uintptr_t addr, size_t size)
    {
        const memory_map_entry* entry = get_memory_map(addr);
        if(entry == nullptr)
        {
            error$("invalid addr: {}", addr);
            exit(3);
        }
        if(entry->end < addr+size)
        {
            error$("invalid addr-size: {}-{}", addr, size);
            exit(3);
        }

        return (void*)((uint8_t*)entry->data + (addr - entry->start));
    }  
}
