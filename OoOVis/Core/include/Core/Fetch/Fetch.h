#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <Frontend/Parser/Instruction.h>

namespace OoOVis
{
    namespace Core
    {
        struct Fetch_Group {
            const std::pair<std::unique_ptr<FrontEnd::Instruction>*, memory_addr_t> first_instruction{};
            const std::pair<std::unique_ptr<FrontEnd::Instruction>*, memory_addr_t> second_instruction{};
            const std::pair<std::unique_ptr<FrontEnd::Instruction>*, memory_addr_t> third_instruction{};
            Fetch_Group(
                const std::pair<std::unique_ptr <FrontEnd::Instruction>*,memory_addr_t>  first_,
                const std::pair<std::unique_ptr <FrontEnd::Instruction>*,memory_addr_t>  second_,
                const std::pair<std::unique_ptr <FrontEnd::Instruction>*,memory_addr_t>  third_
            ) : first_instruction(first_), second_instruction(second_), third_instruction(third_) {}
        };
        class Fetch_Unit {

        public:
            static void                init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions);
            static Fetch_Group          fetch();
            static void                stall();
            static void                continue_fetching();
            static void                increment_counter_by(memory_addr_t value);
            static bool                get_prediction(memory_addr_t branch_instruction_id);
            static void                set_program_counter(memory_addr_t next_pc);
            static memory_addr_t        get_program_counter();
            static memory_addr_t        get_target_addr_from_btb(memory_addr_t branch_instruction_id);
            static void                create_btb_entry(memory_addr_t branch_instruction_id, memory_addr_t target_instruction_id);
            static bool                has_btb_entry(memory_addr_t branch_instruction_id);
            static void                update_pattern_history_table(memory_addr_t branch_instruction_id, bool actual);
        private:
            static std::vector<std::unique_ptr<FrontEnd::Instruction>> _instruction_cache;
            static std::unordered_map<memory_addr_t, memory_addr_t>    _branch_target_buffer;
            static std::unordered_map<u32, u32>                        _pattern_history_table;
            static memory_addr_t                                       _program_counter;
            static memory_addr_t                                       _branch_shift_register;
            static bool _stalled;
        };
        
    } // namespace Core
    
    
} // namespace OoOVis
