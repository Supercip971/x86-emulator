#include "loader_64.hpp"
#include <string.h>
#include "utils/log.hpp"
namespace fp
{

void x86_64_loader::load_entry(memory_map *target, Elf64_Phdr *header, char *data)
{
    if (header->p_type != PT_LOAD)
    {
        debug$("not loading entry: {}", header->p_type);
        return;
    }

    size_t size = std::max(header->p_memsz, header->p_filesz);

    void *virt_data = malloc(size);

    memset(virt_data, 0, size);

    memcpy(virt_data, data + header->p_offset, header->p_filesz);

    target->map_memory(header->p_vaddr, size, virt_data);

    free(virt_data);
}

void x86_64_loader::load(memory_map *target, char *data)
{
    _header = (Elf64_Ehdr *)data;

    if (check_header() != true)
    {
        error$("invalid header, exiting...");
        exit(2);
    }

    Elf64_Phdr *p_header = (Elf64_Phdr *)((uint8_t *)data + _header->e_phoff);

    for (size_t i = 0; i < _header->e_phnum; i++)
    {

        load_entry(target, p_header, data);

        p_header = (Elf64_Phdr *)((uint8_t *)p_header + _header->e_phentsize);
    }
}

bool x86_64_loader::check_header()
{
    if (strncmp((char *)_header->e_ident, ELFMAG, SELFMAG) != 0)
    {
        error$("invalid header identifier");
        return false;
    }

    return true;
}

template <>
loader *loader::load_data<x86_64_loader>(memory_map *target, char *data)
{
    x86_64_loader *res = new x86_64_loader();
    res->load(target, data);
    return res;
}

uintptr_t x86_64_loader::start_addr()
{
    return _header->e_entry;
}

} // namespace fp
