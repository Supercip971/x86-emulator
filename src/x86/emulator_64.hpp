#pragma once
#include <vector>
#include "emulator.hpp"
#include "x86/decoder_64.hpp"
namespace fp
{

enum class cpu_register
{
    CPU_REG_RAX = 0,
    CPU_REG_RCX,
    CPU_REG_RDX,
    CPU_REG_RBX,
    CPU_REG_RSP,
    CPU_REG_RBP,
    CPU_REG_RSI,
    CPU_REG_RDI,
};

struct cpu_registers
{
    union
    {
        struct
        {
            uint64_t rax, rbx, rcx, rdx, rdi, rsi, rbp, rsp;
            uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
            uint64_t rflags, rip;
            uint64_t prip;
        };
        uint64_t regs[20];
    };

    uint64_t &get_register(int id)
    {
        return regs[id];
    }

    uint64_t get_register(int id) const
    {
        return regs[id];
    }
};

class x86_emulator : public emulator, x86_instructions_handler
{
    uint8_t next_byte();
    cpu_registers registers;

    int ins_invalid(x86_instruction &instruction) final;
    int ins_endbr64(x86_instruction &instruction) final;
    int ins_xor(x86_instruction &instruction) final;

    void write_op_rm(uint64_t value, const x86_instruction &instruction);
    uint64_t read_op_rm(const x86_instruction &instruction);

    void write_op_reg(uint64_t value, const x86_instruction &instruction);
    uint64_t read_op_reg(const x86_instruction &instruction);

public:
    uint8_t fetch_byte() final;
    x86_emulator(){};

    virtual void start() final;
};

template <>
emulator *emulator::load_emulator<x86_emulator>(memory_map *mem_map, loader *loader);
} // namespace fp
