#pragma  once

#include <Core/Types/Types.h>
namespace OoOVis
{
	namespace Core {
		struct Reorder_Buffer_Entry {
			bool ready = false;
			bool speculative = false;
			reg_id_t old_alias = 0;
			u32  store_id = NO_STORE;
		};
	}
}
