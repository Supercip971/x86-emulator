#include "x86/emulator_64.hpp"
#include "utils/log.hpp"

namespace fp
{

uint8_t x86_emulator::fetch_byte()
{
    uint8_t r = next_byte();

    registers.rip += 1;
    return r;
}

uint8_t x86_emulator::next_byte()
{
    uint8_t r = _map->read<uint8_t>(registers.rip);
    return r;
}

void x86_emulator::start()
{
    log$("started x86 emulator with starting addr: {}", _loader->start_addr());
    registers.rip = _loader->start_addr();
    x86_decoder dec;
    while (true)
    {
        registers.prip = registers.rip;
        if (dec.run_handler(this) == 0)
        {
            return;
        }
    }
}

template <>
emulator *emulator::load_emulator<x86_emulator>(memory_map *mem_map, loader *loader)
{
    x86_emulator *em = new x86_emulator();
    em->_loader = loader;
    em->_map = mem_map;

    return em;
}
} // namespace fp
