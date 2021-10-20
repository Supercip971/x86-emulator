#include "x86/emulator_64.hpp"

namespace fp
{

void x86_emulator::write_op_rm(uint64_t value, const x86_instruction &instruction)
{
    auto mod = instruction.mod_rm.get_mod();

    if (mod == 3)
    {
        registers.get_register(instruction.mod_rm.get_rm()) = value;
    }
    else
    {
        error$("mod: {} not supported", mod);
    }
}

uint64_t x86_emulator::read_op_rm(const x86_instruction &instruction)
{
    return registers.get_register(instruction.mod_rm.get_rm());
}

void x86_emulator::write_op_reg(uint64_t value, const x86_instruction &instruction)
{
    auto mod = instruction.mod_rm.get_mod();

    if (mod == 3)
    {
        registers.get_register(instruction.mod_rm.get_reg()) = value;
    }
    else
    {
        error$("mod: {} not supported", mod);
    }
}

uint64_t x86_emulator::read_op_reg(const x86_instruction &instruction)
{
    return registers.get_register(instruction.mod_rm.get_reg());
}

int x86_emulator::ins_invalid(x86_instruction &instruction)
{
    error$("invalid/unsupported instruction: (at: {:#x})", registers.prip);

    log$("instruction info:");
    instruction.dump();

    return 0;
}

int x86_emulator::ins_endbr64([[maybe_unused]] x86_instruction &instruction)
{

    log$("unsupported endbr instruction");
    return 1;
}

int x86_emulator::ins_xor(x86_instruction &instruction)
{
    if (instruction.encoding == x86_op_encoding::MODRM_MODREG)
    {
        log$("xor [rm reg]: {} {}", instruction.mod_rm.get_rm(), instruction.mod_rm.get_reg());

        uint64_t a = read_op_rm(instruction);
        uint64_t b = read_op_reg(instruction);

        write_op_rm(a ^ b, instruction);

        return 1;
    }
    log$("xor instruction: {} - {} - {}", instruction.mod_rm.get_mod(), instruction.mod_rm.get_reg(), instruction.mod_rm.get_rm());
    return 0;
}

int x86_emulator::ins_mov(x86_instruction &instruction)
{
    if (instruction.encoding == x86_op_encoding::MODRM_MODREG)
    {
        log$("mov [rm reg]: {} {}", instruction.mod_rm.get_rm(), instruction.mod_rm.get_reg());

        uint64_t b = read_op_reg(instruction);

        write_op_rm(b, instruction);

        return 1;
    }
    log$("mov instruction: {} - {} - {}", instruction.mod_rm.get_mod(), instruction.mod_rm.get_reg(), instruction.mod_rm.get_rm());
    return 0;
}

int x86_emulator::ins_push(x86_instruction &instruction)
{
    if (instruction.encoding == x86_op_encoding::RD)
    {
        log$("push instruction ({}) not supported !", instruction.rd);

        return 1;
    }
    log$("push instruction: {}", instruction.rd);
    return 0;
}

} // namespace fp
