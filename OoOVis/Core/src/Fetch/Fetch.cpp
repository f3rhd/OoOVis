#include <Core/Fetch/Fetch.h>
#include <Core/Constants/Constants.h>
namespace OoOVis
{
    namespace Core
    {
        memory_addr_t Fetch_Unit::_program_counter{};

        std::vector<std::unique_ptr<FrontEnd::Instruction>> Fetch_Unit::_instruction_cache{  };
        bool Fetch_Unit::_stalled{};
		void Fetch_Unit::init(std::vector<std::unique_ptr<FrontEnd::Instruction>>&& instructions)
		{
            _instruction_cache =  std::move(instructions);
		}

		Fetch_Group Fetch_Unit::fetch() {
            std::unique_ptr<FrontEnd::Instruction>* first{ nullptr };
            std::unique_ptr<FrontEnd::Instruction>* second{ nullptr };
            std::unique_ptr<FrontEnd::Instruction>* third{ nullptr };
            if (_program_counter >= _instruction_cache.size() && !_stalled) {
                _stalled = true;
                return { first,second,third };
            }
            if (_program_counter < _instruction_cache.size()) {
                first = &_instruction_cache[_program_counter];
            }
            if (_program_counter + 1 < _instruction_cache.size()) {
                second = &_instruction_cache[_program_counter + 1];
            }
            if (_program_counter + 2 < _instruction_cache.size()) {
                third = &_instruction_cache[_program_counter + 2];
            }
            if (!_stalled) {
                _program_counter += 3;
            }
            return { first,second,third };
        }
        void Fetch_Unit::stall() {
            _stalled = true;
        }
        
		void Fetch_Unit::set_program_counter(memory_addr_t next) {
            _program_counter = next;
		}

		memory_addr_t Fetch_Unit::get_program_counter() {
            return _program_counter;
		}

	} // namespace Core
    
    
} // namespace OoOVis