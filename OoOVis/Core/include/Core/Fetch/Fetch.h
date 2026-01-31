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
            Fetch_Unit(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions) : _instruction_cache(std::move(instructions)) {}
            Fetch_Group fetch();
            void        stall();
        private:
            std::vector<std::unique_ptr<FrontEnd::Instruction>> _instruction_cache;
            uint64_t _program_counter = 0;
            bool _stalled = false;
        };
        
    } // namespace Core
    
    
} // namespace OoOVis
