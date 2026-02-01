#include <Core/Commit/ReorderBufferEntry.h>
#include <memory>
namespace OoOVis 
{
	namespace Core 
	{
		class Reorder_Buffer {

		public:
			static size_t allocate(std::unique_ptr<Reorder_Buffer_Entry>&& entry);
			static void   set_ready(u32 target_entry_index);
			static bool   full();
		private:
			static std::vector<std::unique_ptr<Reorder_Buffer_Entry>> _buffer;
			static size_t _head; // points to the slot that is gonna be retired (if ready) next cycle
			static size_t _tail; // points to the slot that is going to be allocated 
		};
	}
}