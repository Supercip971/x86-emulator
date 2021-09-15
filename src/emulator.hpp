#pragma once 
#include "config.hpp"
#include "loader.hpp"
namespace fp 
{

class emulator 
{
    protected:
    memory_map* _map;
    loader* _loader;
    public: 

    void set_memory_map(memory_map* map)
    {
        _map = map;
    }

    virtual void start() = 0;

    template<typename T>
    static emulator* load_emulator([[maybe_unused]]memory_map* mem_map, loader* loader){return nullptr;};
};
}
