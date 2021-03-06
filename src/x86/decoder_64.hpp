#pragma once
#include <stdint.h>
#include <vector>
namespace fp
{

enum class x86_prefixes : uint8_t
{
    REPN = 0xf2,
    REPZ = 0xf3
};

enum class x86_op_encoding : uint8_t
{
    IMMEDIATE = 1,    // i
    MODRM_IMM = 2,    // mi
    MODRM_MODREG = 3, // mr
    MODREG_MODRM = 4, // rm
    RD = 5,
};

struct x86_rex_prefix
{
    bool w;
    bool r;
    bool x;
    bool b;
};

struct x86_mod_rm
{
    uint8_t value;
    uint64_t disp;
    uint8_t mod;
    uint8_t rm;
    uint8_t reg;

    bool sib_follows;

    bool has_sib() const
    {
        return sib_follows;
    }

    uint8_t get_mod() const
    {
        return mod;
    }

    uint8_t get_rm() const
    {
        return rm;
    }

    uint8_t get_reg() const
    {
        return reg;
    }

    uint64_t get_disp() const
    {
        return disp;
    }
};
class x86_instruction
{
public:
    bool has_rex;
    x86_rex_prefix rex;
    std::vector<uint8_t> ins_stack;
    std::vector<x86_prefixes> ins_prefixes;
    std::vector<uint8_t> opcodes;

    bool has_mod_rm;
    
    x86_op_encoding encoding;
    union 
    {
        x86_mod_rm mod_rm;
    
        int rd;
    };
    void dump();
};

class x86_instructions_handler
{
public:
    virtual uint8_t fetch_byte() = 0;
    virtual uint8_t next_byte() = 0;
    virtual int ins_endbr64(x86_instruction &) = 0;
    virtual int ins_invalid(x86_instruction &instruction) = 0;
    virtual int ins_xor(x86_instruction &instruction) = 0;
    virtual int ins_mov(x86_instruction &instruction) = 0;
    virtual int ins_push(x86_instruction &instruction) =0;
};

class x86_decoder
{
    x86_instruction curr_decoded_ins;
    uint8_t fetch_byte(x86_instructions_handler *handler)
    {
        uint8_t v = handler->fetch_byte();
        curr_decoded_ins.ins_stack.push_back(v);
        return v;
    }

    int interp_operand(x86_instructions_handler *handler);
    bool is_string_opcode(uint8_t op);
    int interp_opcode_endbr(x86_instructions_handler *handler);
    int interp_opcode_0f(x86_instructions_handler *handler);
    int interp_xor_opcodes(x86_instructions_handler *handler);
    int interp_mov_opcodes(x86_instructions_handler *handler);
    int interp_opcode(x86_instructions_handler *handler);
    int interp_prefix(x86_instructions_handler *handler);

public:
    int run_handler(x86_instructions_handler *handler);
};

} // namespace fp
