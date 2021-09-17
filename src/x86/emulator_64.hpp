#pragma once
#include "emulator.hpp"
#include "x86/decoder_64.hpp"
#include <vector>
namespace fp 
{
    enum class x86_ins 
    {

    };
    struct cpu_registers
    {
        uint64_t rax, rbx, rcx, rdx, rdi, rsi, rbp, rsp;
        uint64_t r8, r9, r10, r11, r12, r13,r14,r15;
        uint64_t rflags, rip; 
        uint64_t prip;
    };
    class x86_emulator : public emulator, x86_instructions_handler
    {

        
        uint8_t next_byte();
        cpu_registers registers;


        int ins_invalid(x86_instruction& instruction) final;
        int ins_endbr64(x86_instruction& instruction) final;
        public:

        uint8_t fetch_byte() final;
        x86_emulator(){};

        virtual void start() final;
    };
    
    template<>
    emulator* emulator::load_emulator<x86_emulator>(memory_map* mem_map, loader* loader);
}
