#pragma once

#include <memory>
#include <vector>

#include <Frontend/Parser/Instruction.h>

namespace OoOVis
{
    namespace Core
    {
        auto constexpr FETCH_WIDTH = 3;
        struct Fetch_Group {
            const std::unique_ptr<Frontend::Instruction>* first = nullptr;
            const std::unique_ptr<Frontend::Instruction>* second = nullptr;
            const std::unique_ptr<Frontend::Instruction>* third = nullptr;
            Fetch_Group(
                const std::unique_ptr <Frontend::Instruction>* first_,
                const std::unique_ptr <Frontend::Instruction>* second_,
                const std::unique_ptr <Frontend::Instruction>* third_
            ) : first(first_), second(second_), third(third_) {}
        };
        class Fetch_Unit {

        public:
            Fetch_Unit(std::vector<std::unique_ptr<Frontend::Instruction>>&& instructions) : _instruction_cache(std::move(instructions)) {}
            Fetch_Group fetch();
            void        stall();
        private:
            std::vector<std::unique_ptr<Frontend::Instruction>> _instruction_cache;
            uint64_t _program_counter = 0;
            bool _stalled = false;
        };
        
    } // namespace Core
    
    
} // namespace OoOVis
