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

    class x86_instruction 
    {
        public:

        std::vector<uint8_t> ins_stack;
        std::vector<x86_prefixes> ins_prefixes;
        std::vector<uint8_t> opcodes;
    };
    class x86_instructions_handler
    {
        public: 
        virtual uint8_t fetch_byte() = 0;
        virtual uint8_t next_byte() = 0;
        virtual int ins_endbr64(x86_instruction&) = 0;
        virtual int ins_invalid(x86_instruction& instruction) = 0;
    };

    
    class x86_decoder
    {
        x86_instruction curr_decoded_ins;
        uint8_t fetch_byte(x86_instructions_handler* handler)
        {
            uint8_t v = handler->fetch_byte();
            curr_decoded_ins.ins_stack.push_back(v);
            return v;
        }
        bool is_string_opcode(uint8_t op); 
        int interp_opcode_endbr(x86_instructions_handler* handler);
        int interp_opcode_0f(x86_instructions_handler* handler);
        int interp_opcode(x86_instructions_handler* handler);
        int interp_prefix(x86_instructions_handler* handler);
        public:

        int run_handler(x86_instructions_handler* handler);        
    };
}
