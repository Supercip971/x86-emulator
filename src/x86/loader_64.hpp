#pragma once 
#include "loader.hpp"
#include <elf.h>

namespace fp 
{
    class x86_64_loader : public loader
    {
        protected:

        
        Elf64_Ehdr* _header; 
        void load_entry(memory_map* mem_map, Elf64_Phdr* header, char* data);
        bool check_header();
        public: 

        virtual uintptr_t start_addr() final;
        virtual void load(memory_map* target, char* data) final;
    };
    
    
    template<>
    loader* loader::load_data<x86_64_loader>(memory_map* target, char* data);
   
}
