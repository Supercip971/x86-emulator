#include "x86/emulator_64.hpp"
namespace fp 
{

int x86_emulator::ins_invalid(x86_instruction& instruction)
{
    error$("invalid/unsupported instruction: (at: {:#x})", registers.prip);

    for(size_t i = 0; i < instruction.ins_stack.size(); i++)
    {
        error$("data[{}] = {:#x}", i, instruction.ins_stack[i]);
    }
    return 0;
}
int x86_emulator::ins_endbr64( [[maybe_unused]] x86_instruction &instruction)
{

    log$("unsupported endbr instruction");
    return 1;
};


}
