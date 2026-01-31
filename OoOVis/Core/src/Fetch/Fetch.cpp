#include <Core/Fetch/Fetch.h>
#include <Core/Constants/Constants.h>
namespace OoOVis
{
    namespace Core
    {
        Fetch_Group Fetch_Unit::fetch() {
            std::unique_ptr<FrontEnd::Instruction>* first = nullptr;
            std::unique_ptr<FrontEnd::Instruction>* second = nullptr;
            std::unique_ptr<FrontEnd::Instruction>* third = nullptr;
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
        
    } // namespace Core
    
    
} // namespace OoOVis