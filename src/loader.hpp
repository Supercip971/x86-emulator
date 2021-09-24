#pragma once
#include "config.hpp"
#include "memory.hpp"
namespace fp
{

class loader
{
protected:
    char *_raw;
    archs _arch;

public:
    virtual void load(memory_map *target, char *data) = 0;

    virtual uintptr_t start_addr() = 0;
    archs loader_arch() const { return _arch; };

    template <typename type>
    static loader *load_data([[maybe_unused]] memory_map *target, [[maybe_unused]] char *data);
};

} // namespace fp
