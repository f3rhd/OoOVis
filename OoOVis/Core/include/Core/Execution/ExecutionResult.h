#pragma  once
#include <Core/Types/Types.h>
namespace OoOVis
{
	namespace Core
	{
		/*
		some instructions will generate this kind of layout upon execution and will be sent to common data bus, then the results that are
		chosen by the arbiter will be used to set ready the rob entries and update the physical register file
		load instruction wont depend on the arbiter, meaning it can directly update the physical register file itself and 
		at the same time set the respectful rob entry ready
		*/
		struct Execution_Result {
			bool		valid{ false };
			data_t		write_data{};
			u64			target_reorder_buffer_entry_index{};
			u32			producer_tag{};
			reg_id_t    destination_physical_register{};
		};
	}
}