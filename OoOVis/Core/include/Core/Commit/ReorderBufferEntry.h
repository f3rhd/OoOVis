#pragma  once

#include <Core/Types/Types.h>
namespace OoOVis
{
	namespace Core {
		struct Reorder_Buffer_Entry {
			reg_id_t old_alias = 0;
			bool speculative = false;
		};
	}
}
