#include <Core/Fetch/Fetch.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Constants/Constants.h>
namespace OoOVis
{
    namespace Core
    {
        memory_addr_t Fetch_Unit::_program_counter{};
        std::vector<std::unique_ptr<FrontEnd::Instruction>> Fetch_Unit::_instruction_cache{};
        std::unordered_map<memory_addr_t, memory_addr_t>    Fetch_Unit::_branch_target_buffer{};
        std::unordered_map<u32, u32>                        Fetch_Unit::_pattern_history_table{};
        bool Fetch_Unit::_next_fetch_is_set{ false };

        fetch_group_t Fetch_Group::group{};
        memory_addr_t Fetch_Unit::_branch_shift_register{};
        bool Fetch_Unit::next_fetch_is_set() {
            return _next_fetch_is_set;
        }
        void Fetch_Unit::init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions) {
            _instruction_cache = std::move(instructions);
        }

		bool Fetch_Unit::get_prediction(memory_addr_t branch_instruction_id) {
            memory_addr_t pht_index{ _branch_shift_register ^ branch_instruction_id };
			if(_pattern_history_table.find(pht_index) == _pattern_history_table.end()) {
				_pattern_history_table.emplace(pht_index, 2); // start at weakly taken
				return true;
			}
			u32 branch_history = _pattern_history_table[pht_index];
			if(branch_history >= 2)
				return true;
			return false;
		}

		void Fetch_Unit::set_program_counter(memory_addr_t next) {
            _next_fetch_is_set = true;
            _program_counter = next;
        }

        memory_addr_t Fetch_Unit::get_program_counter() {
            return _program_counter;
        }

        void Fetch_Unit::create_btb_entry(memory_addr_t branch_instruction_id, memory_addr_t target_instruction_id) {
            _branch_target_buffer.insert_or_assign(branch_instruction_id, target_instruction_id);
        }

        memory_addr_t Fetch_Unit::get_target_addr_from_btb(memory_addr_t branch_instruction_id) {
            return _branch_target_buffer.at(branch_instruction_id);
        }
		bool Fetch_Unit::has_btb_entry(memory_addr_t branch_instruction_id) {
            return _branch_target_buffer.find(branch_instruction_id) != _branch_target_buffer.end();

		}

		void Fetch_Unit::update_pattern_history_table(memory_addr_t branch_instruction_id, bool actual) {
            u32 pht_index = _branch_shift_register ^ branch_instruction_id;
			u32 branch_history = _pattern_history_table[pht_index];
			
			if (actual) {
				if (branch_history >= 0 && branch_history < 3)
					branch_history++;
			}
			else {
				if (branch_history > 0) 
					branch_history--;
			}
			
			_pattern_history_table[pht_index] = branch_history;
			
			_branch_shift_register <<= 1;
			if (actual) {
				_branch_shift_register |= 1;
			}
			_branch_shift_register &= (1 << Constants::BRANCH_SHIFT_REGISTER_SIZE) - 1;
		}

		fetch_group_t Fetch_Unit::fetch() {
            fetch_group_t fetch_group(Constants::FETCH_WIDTH);
            if (_program_counter >= _instruction_cache.size()) {
                return fetch_group;
            }
            memory_addr_t temp_address{ _program_counter };
            for (size_t i{}; i < Constants::FETCH_WIDTH; i++) {
                _next_fetch_is_set = false;
                if (temp_address < _instruction_cache.size()) {
                    fetch_group[i].first = &_instruction_cache[temp_address];
                    fetch_group[i].second = temp_address;
                    if (fetch_group[i].first->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL) {
                        if (has_btb_entry(fetch_group[i].second)) {
                            _program_counter = get_target_addr_from_btb(fetch_group[i].second);
                            _next_fetch_is_set = true;
                            break;
                        }
                    }
                    else if (fetch_group[i].first->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL) {
                        if (has_btb_entry(fetch_group[i].second)) {
							memory_addr_t target_address{ get_target_addr_from_btb(fetch_group[i].second) };
							if (get_prediction(fetch_group[i].second)) {
								_program_counter = target_address;
								_next_fetch_is_set = true;
								break;
							}
                        }
                    }
                }
                temp_address++;
            }

			return fetch_group;
        }


		void Fetch_Unit::adjust_program_counter_based_on_successful_dispatches(memory_addr_t amount) {
            _program_counter +=  amount;
		}

	} // namespace Core


} // namespace OoOVis