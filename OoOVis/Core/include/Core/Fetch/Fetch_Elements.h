#pragma  once
#include <memory>
#include <Frontend/Parser/Instruction.h>
#include <Core/Types/Types.h>
#include <Core/Constants/Constants.h>
namespace OoOVis
{
	namespace Core
	{
        struct Fetch_Element {
            std::unique_ptr<FrontEnd::Instruction>* instruction;
            memory_addr_t address;
            u32 speculation_id = Constants::NOT_SPECULATIVE;
        };
        struct Fetch_Group {
            static std::vector<Fetch_Element> group;
        };

	}
}