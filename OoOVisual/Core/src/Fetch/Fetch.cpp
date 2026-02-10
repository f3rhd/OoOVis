#include <Core/Fetch/Fetch.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Constants/Constants.h>
namespace OoOVisual
{
    namespace Core
    {
        memory_addr_t Fetch_Unit::_program_counter{};
        std::vector<std::unique_ptr<FrontEnd::Instruction>> Fetch_Unit::_instruction_cache{};
        std::unordered_map<memory_addr_t, memory_addr_t>    Fetch_Unit::_branch_target_buffer{};
        std::unordered_map<u32, u32>                        Fetch_Unit::_pattern_history_table{};
        time_t                                              Fetch_Unit::_timestamp{ 0};
        bool                                                Fetch_Unit::_next_fetch_is_set{ false };
        memory_addr_t                                       Fetch_Unit::_last_branch_timestamp = Constants::NOT_SPECULATIVE;
        memory_addr_t                                       Fetch_Unit::_branch_shift_register{};
        std::vector<Fetch_Element>                          Fetch_Unit::_last_fetch_group{};
        std::vector<Fetch_Element>                          Fetch_Group::group{};

        std::vector<Fetch_Element> Fetch_Unit::fetch(const std::vector<DISPATCH_FEEDBACK>& dispatch_feedback) {
			_timestamp += Constants::UNIT_TIME;
            std::vector<Fetch_Element> fetch_group(Constants::FETCH_WIDTH);
            if (_program_counter >= _instruction_cache.size()) {
                _last_fetch_group = fetch_group;
                return fetch_group;
            }
            for (size_t i{}; i < Constants::FETCH_WIDTH; i++) {
                switch (dispatch_feedback[i])
                {
                case OoOVisual::Core::DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH:
                    break;
                case OoOVisual::Core::DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL:
                case OoOVisual::Core::DISPATCH_FEEDBACK::REGISTER_FILE_WAS_FULL:
                case OoOVisual::Core::DISPATCH_FEEDBACK::REORDER_BUFFER_WAS_FULL:
                    _program_counter = _last_fetch_group[i].address;
                    goto end_of_loop;
                case OoOVisual::Core::DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH:
                    if (!_next_fetch_is_set)
                        _program_counter++;
                    break;
                default:
                    break;
                }
            }
            end_of_loop:
            memory_addr_t temp_address{ _program_counter };
            for (size_t i{}; i < Constants::FETCH_WIDTH; i++) {
                _next_fetch_is_set = false;
                auto& fetch_element = fetch_group[i];
                if (temp_address < _instruction_cache.size()) {
                    fetch_element.instruction = &_instruction_cache[temp_address];
                    fetch_element.address = temp_address;
                    fetch_element.origin_branch_timestamp = _last_branch_timestamp;
                    fetch_element.timestamp = _timestamp;
                    if (fetch_element.instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL) {
                        switch (dynamic_cast<FrontEnd::Jump_Instruction*>(fetch_element.instruction->get())->kind()) {
                        case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JAL: // this one immediately jumps to the label
                            _program_counter = fetch_element.instruction->get()->target_addr();
                            _next_fetch_is_set = true;
                            _last_fetch_group = fetch_group;
                            return fetch_group;
                        case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JALR: 
                            /* jalr is always wrong predicted so we have to seperate its timestamp from incoming instructions*/
							_last_branch_timestamp = _timestamp;
							_timestamp += Constants::UNIT_TIME;
                            break;
					    /*JALR instruction uses rs1 and imm value to calculate the target address we cant do that here we are gonna let 
                        the fetch unit to continue fetching from wrong control block we are going to do recovery in the execution phase*/
                        default: // wont happen
                            break;
                        }
                    }
                    else if (fetch_element.instruction->get()->flow() == FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL) {
						_last_branch_timestamp = _timestamp;
                        if (has_btb_entry(fetch_element.address)) {
							memory_addr_t target_address{ get_target_addr_from_btb(fetch_element.address) };
							if (get_prediction(fetch_element.address)) {
								_program_counter = target_address;
								_next_fetch_is_set = true;
                                fetch_element.branch_prediction = Constants::PREDICTED_TAKEN;
								_last_fetch_group = fetch_group;
                                return fetch_group;
							}
                        }
						fetch_element.branch_prediction = Constants::PREDICTED_NOT_TAKEN;
                        /* to make flushing of reservation station entries easier we seperate the timestamp
                        of this intruction from incoming instructions (this is needed when branch is predicted to be not taken) 
                        so when they are in the same fetch group later instructions will get different timestamp*/
						_timestamp += Constants::UNIT_TIME;;
                    }
                }
                temp_address++;
            }
			_last_fetch_group = fetch_group;
			return fetch_group;
        }

        void Fetch_Unit::init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions) {
            _instruction_cache = std::move(instructions);
        }

        bool Fetch_Unit::get_prediction(memory_addr_t branch_instruction_id) {
            memory_addr_t pht_index{ _branch_shift_register ^ branch_instruction_id };
            if (_pattern_history_table.find(pht_index) == _pattern_history_table.end()) {
                _pattern_history_table.emplace(pht_index, 2); // start at weakly taken
                return true;
            } 
            u32 branch_history = _pattern_history_table[pht_index];
            if (branch_history >= 2)
                return true;
            return false;
        }

        void Fetch_Unit::set_program_counter_flags() {

            _last_branch_timestamp = Constants::NOT_SPECULATIVE;
            _next_fetch_is_set = true;
        }
        void Fetch_Unit::set_program_counter(memory_addr_t next) {
            _program_counter = next;
        }

		void Fetch_Unit::reset_speculation_id() {
            _last_branch_timestamp = Constants::NOT_SPECULATIVE;
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


		void Fetch_Unit::adjust_program_counter_based_on_successful_dispatches(memory_addr_t amount) {
            _program_counter +=  amount;
		}

	} // namespace Core


} // namespace OoOVis