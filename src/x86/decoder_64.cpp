#include "x86/decoder_64.hpp"

namespace fp 
{

int x86_decoder::interp_opcode_endbr(x86_instructions_handler* handler)
{
    uint8_t byte = fetch_byte(handler);

    if (byte == 0xfa)
    {
        handler->ins_endbr64(curr_decoded_ins);
        return 1;
    }
    else
    {
        handler->ins_invalid(curr_decoded_ins);
        return 0;
    }
}
bool x86_decoder::is_string_opcode([[maybe_unused]] uint8_t op)
{
    return false;
}
int x86_decoder::interp_opcode_0f(x86_instructions_handler* handler)
{
    uint8_t byte = fetch_byte(handler);
    if (byte == 0x1e)
    {
        return interp_opcode_endbr(handler);
    }
    else 
    {
         handler->ins_invalid(curr_decoded_ins);
       
    }
    return 0;
}
int x86_decoder::interp_opcode(x86_instructions_handler* handler)
{
    uint8_t byte = fetch_byte(handler);
    if (byte == 0x0f)
    {
        return interp_opcode_0f(handler);
    }
    else 
    {
         handler->ins_invalid(curr_decoded_ins);
       
    }
    return 0;
}
int x86_decoder::interp_prefix(x86_instructions_handler* handler)
{
    uint8_t cur = handler->next_byte();

    if (cur == (uint8_t)x86_prefixes::REPZ)
    { 
        cur = handler->fetch_byte();
        if (is_string_opcode(handler->next_byte()))
        {
            curr_decoded_ins.ins_prefixes.push_back(x86_prefixes::REPZ);
            return interp_prefix(handler);
        }

        return interp_opcode(handler);
    }

    return interp_opcode(handler);
}

int x86_decoder::run_handler(x86_instructions_handler* handler)
{
    curr_decoded_ins.ins_prefixes.clear();
    curr_decoded_ins.ins_stack.clear();
    curr_decoded_ins.opcodes.clear();
    return interp_prefix(handler);
}
}
