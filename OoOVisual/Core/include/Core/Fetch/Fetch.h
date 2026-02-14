#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <Frontend/Parser/Instruction.h>
#include <Core/Fetch/FetchElements.h>
#include <Core/Dispatch/Dispatcher.h>

namespace OoOVisual
{
    namespace Core
    {
        class Fetch_Unit {

        public:
            static void                             init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions);
            static std::vector<Fetch_Element>       fetch(const std::vector<DISPATCH_FEEDBACK>& dispatch_feedback );
            static void                             adjust_program_counter_based_on_successful_dispatches(memory_addr_t amount);
            static bool                             get_prediction(memory_addr_t branch_instruction_id);
            static void                             set_program_counter(memory_addr_t next_pc);
            static memory_addr_t                    get_program_counter();
            static memory_addr_t                    get_target_addr_from_btb(memory_addr_t branch_instruction_id);
            static void                             create_btb_entry(memory_addr_t branch_instruction_id, memory_addr_t target_instruction_id);
            static bool                             has_btb_entry(memory_addr_t branch_instruction_id);
            static void                             update_pattern_history_table(memory_addr_t branch_instruction_id, bool actual);
            static void                             set_program_counter_flags();
        private:
            static bool                                                 _next_fetch_is_set;
            static std::vector<std::unique_ptr<FrontEnd::Instruction>> _instruction_cache;
            static std::unordered_map<memory_addr_t, memory_addr_t>    _branch_target_buffer;
            static std::unordered_map<u32, u32>                        _pattern_history_table;
            static memory_addr_t                                       _program_counter;
            static memory_addr_t                                       _branch_shift_register;
            static time_t                                              _timestamp;
            static std::vector<Fetch_Element>                          _last_fetch_group;
        };
        
    } // namespace Core
    
    
} // namespace OoOVis
