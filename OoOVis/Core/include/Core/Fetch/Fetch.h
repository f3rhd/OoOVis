#pragma once

#include <memory>
#include <vector>

#include <Frontend/Parser/Instruction.h>

namespace OoOVis
{
    namespace Core
    {
        struct Fetch_Group {
            const std::unique_ptr<FrontEnd::Instruction>* first = nullptr;
            const std::unique_ptr<FrontEnd::Instruction>* second = nullptr;
            const std::unique_ptr<FrontEnd::Instruction>* third = nullptr;
            Fetch_Group(
                const std::unique_ptr <FrontEnd::Instruction>* first_,
                const std::unique_ptr <FrontEnd::Instruction>* second_,
                const std::unique_ptr <FrontEnd::Instruction>* third_
            ) : first(first_), second(second_), third(third_) {}
        };
        class Fetch_Unit {

        public:
            static void                init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions);
            static Fetch_Group         fetch();
            static void                stall();
            static void                set_program_counter(memory_addr_t next_pc);
            static memory_addr_t       get_program_counter();
        private:
            static std::vector<std::unique_ptr<FrontEnd::Instruction>> _instruction_cache;
            static memory_addr_t _program_counter;
            static bool _stalled;
        };
        
    } // namespace Core
    
    
} // namespace OoOVis
