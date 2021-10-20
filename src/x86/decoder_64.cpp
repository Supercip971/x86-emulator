#include "x86/decoder_64.hpp"
#include "utils/log.hpp"
namespace fp
{

void x86_instruction::dump()
{
    log$("prefixes: ");
    if (has_rex)
    {
        debug$("rex prefix: ");

        debug$("- w: {}", rex.w);
        debug$("- r: {}", rex.r);
        debug$("- x: {}", rex.x);
        debug$("- b: {}", rex.b);
    }
    for (size_t i = 0; i < ins_prefixes.size(); i++)
    {
        if (ins_prefixes[i] == x86_prefixes::REPN)
        {
            debug$("- prefix: REPN");
        }
        else if (ins_prefixes[i] == x86_prefixes::REPZ)
        {
            debug$("- prefix: REPZ");
        }
    }

    log$("mod rm: ");

    if (has_mod_rm)
    {
        debug$("- disp: {}", mod_rm.get_disp());
        debug$("- reg: {}", mod_rm.get_reg());
        debug$("- rm: {}", mod_rm.get_rm());
        debug$("- mod: {}", mod_rm.get_mod());
    }

    log$("raw informations:");

    debug$("instruction:");

    for (size_t i = 0; i < ins_stack.size(); i++)
    {
        log$("[{}]: {:#x}", i, ins_stack[i]);
    }
}
int x86_decoder::interp_opcode_endbr(x86_instructions_handler *handler)
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

int x86_decoder::interp_opcode_0f(x86_instructions_handler *handler)
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

int x86_decoder::interp_operand(x86_instructions_handler *handler)
{
    uint8_t byte = fetch_byte(handler);
    x86_mod_rm rm = {};
    rm.value = byte;

    rm.mod = (byte >> 6) & (0b11);

    rm.reg = (byte >> 3) & (0b111);
    rm.rm = byte & 0b111;
    rm.disp = 0;

    if (rm.mod != 0b11 && rm.reg == 0b100)
    {
        rm.sib_follows = true;
    }
    else
    {
        rm.sib_follows = false;
    }
    if (rm.sib_follows)
    {
        error$("sib not supported");
        return 0;
    }
    if (curr_decoded_ins.has_rex)
    {
        rm.rm = rm.rm | ((curr_decoded_ins.rex.b) << 3);
        rm.reg = rm.reg | ((curr_decoded_ins.rex.r) << 3);
        log$("rm: {} reg: {}", rm.rm, rm.reg);
    }

    curr_decoded_ins.mod_rm = rm;

    curr_decoded_ins.has_mod_rm = true;
    return 1;
}

int x86_decoder::interp_xor_opcodes(x86_instructions_handler *handler)
{
    int r = interp_operand(handler);

    if (r == 1)
    {
        return handler->ins_xor(curr_decoded_ins);
    }

    return r;
}

int x86_decoder::interp_mov_opcodes(x86_instructions_handler *handler)
{
    int r = interp_operand(handler);

    if (r == 1)
    {
        return handler->ins_mov(curr_decoded_ins);
    }

    return r;
}
int x86_decoder::interp_opcode(x86_instructions_handler *handler)
{

    uint8_t byte = fetch_byte(handler);
    
    if (byte == 0x0f)
    {
        return interp_opcode_0f(handler);
    }
    else if (byte == 0x31)
    {
        curr_decoded_ins.encoding = x86_op_encoding::MODRM_MODREG;
        return interp_xor_opcodes(handler);
    }
    else if (byte == 0x89)
    {
        curr_decoded_ins.encoding = x86_op_encoding::MODRM_MODREG;
        return interp_mov_opcodes(handler);
    }
    else if((byte & 0b11111000) == 0x58)
    {
        curr_decoded_ins.rd = byte & 0b111;
        curr_decoded_ins.encoding = x86_op_encoding::RD;
        return handler->ins_push(curr_decoded_ins);
    }
    else
    {
        handler->ins_invalid(curr_decoded_ins);
    }

    return 0;
}

int x86_decoder::interp_prefix(x86_instructions_handler *handler)
{
    uint8_t cur = handler->next_byte();

    if (cur == (uint8_t)x86_prefixes::REPZ)
    {
        cur = fetch_byte(handler);
        if (is_string_opcode(handler->next_byte()))
        {
            curr_decoded_ins.ins_prefixes.push_back(x86_prefixes::REPZ);
            return interp_prefix(handler);
        }

        return interp_opcode(handler);
    }
    else if ((cur & 0b11110000) == 0b01000000 && !curr_decoded_ins.has_rex)
    {
        cur = fetch_byte(handler);

        curr_decoded_ins.has_rex = true;

        curr_decoded_ins.rex.b = cur & (1 << 0);
        curr_decoded_ins.rex.x = cur & (1 << 1);
        curr_decoded_ins.rex.r = cur & (1 << 2);
        curr_decoded_ins.rex.w = cur & (1 << 3);
        return interp_prefix(handler);
    }

    return interp_opcode(handler);
}

int x86_decoder::run_handler(x86_instructions_handler *handler)
{
    curr_decoded_ins.has_rex = false;
    curr_decoded_ins.has_mod_rm = false;
    curr_decoded_ins.ins_prefixes.clear();
    curr_decoded_ins.ins_stack.clear();
    curr_decoded_ins.opcodes.clear();
    return interp_prefix(handler);
}

} // namespace fp
