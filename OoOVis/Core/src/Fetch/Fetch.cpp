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
        bool Fetch_Unit::_stalled{};
        memory_addr_t Fetch_Unit::_branch_shift_register{};
        void Fetch_Unit::init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions) {
            _instruction_cache = std::move(instructions);
        }

        void Fetch_Unit::stall() {
            _stalled = true;
        }

        void Fetch_Unit::continue_fetching() {
            _stalled = false;
        }

		void Fetch_Unit::increment_counter_by(memory_addr_t value) {
            _program_counter += value;
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
			_branch_shift_register &= (1 << BRANCH_SHIFT_REGISTER_SIZE) - 1;
		}

		Fetch_Group Fetch_Unit::fetch() {
            std::unique_ptr<FrontEnd::Instruction>* first_instruction{};
            std::unique_ptr<FrontEnd::Instruction>* second_instruction{};
            std::unique_ptr<FrontEnd::Instruction>* third_instruction{};
            memory_addr_t first_addr{};
            memory_addr_t second_addr{};
            memory_addr_t third_addr{};
            memory_addr_t current_address = _program_counter;
            if (current_address >= _instruction_cache.size() && !_stalled) {
                _stalled = true;
				return { {first_instruction,first_addr},{second_instruction,second_addr},{third_instruction,third_addr} };
            }
            if (current_address < _instruction_cache.size()) {
                first_instruction = &_instruction_cache[current_address];
                first_addr = current_address;
				current_address++;
            }
            if (current_address < _instruction_cache.size()) {
                second_instruction = &_instruction_cache[current_address];
                second_addr = current_address;
				current_address++;
            }
            if (current_address < _instruction_cache.size()) {
                third_instruction = &_instruction_cache[current_address];
                third_addr  = current_address;
            }

            if (first_instruction && first_instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL) {
                if (Fetch_Unit::has_btb_entry(first_addr)) {
                    _program_counter = Fetch_Unit::get_target_addr_from_btb(first_addr);
                }
                second_instruction = third_instruction = nullptr;
                second_addr = third_addr =  0;
            }
            else if (second_instruction && second_instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL) {
                if (Fetch_Unit::has_btb_entry(second_addr)) {
                    _program_counter = Fetch_Unit::get_target_addr_from_btb(second_addr);
                }
                third_instruction = nullptr;
                third_addr = 0;
            }
            else if (third_instruction && third_instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL) {
                if (Fetch_Unit::has_btb_entry(third_addr)) {
                    _program_counter = Fetch_Unit::get_target_addr_from_btb(third_addr);
                }
            }
            else if (first_instruction && first_instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL) {
                Register_File::take_snapshot();
                if (Fetch_Unit::has_btb_entry(first_addr)) {
                    memory_addr_t target_address = Fetch_Unit::get_target_addr_from_btb(first_addr);
                    if (get_prediction(first_addr) == true) {
                        _program_counter = target_address;
                        second_instruction = third_instruction = nullptr;
                        second_addr = third_addr = 0;
                    }
                }
            }
            else if (second_instruction && second_instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL) {
                Register_File::take_snapshot();
                if (Fetch_Unit::has_btb_entry(second_addr)) {
                    memory_addr_t target_address = Fetch_Unit::get_target_addr_from_btb(second_addr);
                    if (get_prediction(second_addr) == true) {
                        _program_counter = target_address;
                         third_instruction = nullptr;
                         third_addr = 0;
                    }
                }
            }
            else if (third_instruction && third_instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL) {
                Register_File::take_snapshot();
                if (Fetch_Unit::has_btb_entry(third_addr)) {
                    memory_addr_t target_address = Fetch_Unit::get_target_addr_from_btb(third_addr);
                    if (get_prediction(third_addr) == true) {
                        _program_counter = target_address;
                    }
                }
            }
			return { {first_instruction,first_addr},{second_instruction,second_addr},{third_instruction,third_addr} };
        }


	} // namespace Core


} // namespace OoOVis