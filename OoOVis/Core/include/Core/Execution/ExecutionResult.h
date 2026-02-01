#pragma  once
#include <Core/Types/Types.h>
namespace OoOVis
{
	namespace Core
	{
		struct Execution_Result {
			bool		valid{ false };
			data_t		write_data{};
			u64			target_reorder_buffer_entry_index{};
			u32			producer_tag{};
			reg_id_t    destination_physical_register{};
		};
	}
}